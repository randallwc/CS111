// NAME: William Randall
// EMAIL: wrandall1000@gmail.com
// ID: 805167986

#include <mraa.h> //aio and gpio
#include <stdio.h> //fprintf
#include <stdlib.h> //exit // rand
#include <string.h> //strerror //atoi
#include <getopt.h> //getopt
#include <time.h> //timespec //clock_gettime
#include <poll.h> //poll
#include <fcntl.h> //open //creat //F_SETFL
#include <unistd.h> //close //fork //pipe
#include <math.h> //log
// #include <stdbool.h>

//set up constants for pins
#define TEMP_PIN 1
#define BUTTON_PIN 60

//GLOBAL STRUCT
static struct option arr_option[] =
{
	{"period",	required_argument,		0,	'p'},
	{"scale",	required_argument,		0,	's'},
	{"log",		required_argument,		0,	'l'},
	{0, 0, 0, 0}
};
//--period=#				//default 1(seconds)
//--scale=C (or --scale=F)	//default F
//log=filename				//optional

//flag variables
int p_flag = 0;
int s_flag = 0;
int l_flag = 0;
int run_flag = 1;
int shutdown_flag = 0;

//argument variables
int p_arg 		= 1;
char s_arg		= 'F';
char * l_arg	= NULL;
int logfd 		= -1;

//buffers
char read_buf[256] = {0};
int read_length = 0;

//helper functions
double get_temp(int temp_reading, char scale);
void print_and_log(int hour, int min, int sec, double temperature);

//stdin arguments
/**
	SCALE=F
		all subsequent reports to be generated in degrees Fahrenheit
	SCALE=C
		all subsequent reports to be generated in degrees Centegrade
	PERIOD=seconds
		change the number of seconds between reporting intervals
		It is acceptable if this command does not take effect until after the next report.
	STOP
		cause the program to stop generating reports, but continue processing input commands.
		If the program is not generating reports, merely log receipt of the command.
	START
		cause the program to, if stopped, resume generating reports.
		If the program is not stopped, merely log receipt of the command.
	LOG line of text
		This command requires no action beyond logging its receipt (the entire command, including the LOG).
	OFF
		(like pressing the button) output (and log) a time-stamped SHUTDOWN message and exit.
*/

//ex output:
//"17:25:58 98.6\n"
//write to fd 1

int main(int argc, char ** argv){

	int ind = 0;
	int ch = 0;
	//argument parsing
	while ( (ch = getopt_long(argc, argv, "p:s:l:", arr_option, &ind) ) != -1){
		switch (ch){
			case 'p':
				p_flag = 1;
				p_arg = atoi(optarg);
				break;
			case 's':
				s_flag = 1;
				s_arg = *optarg;
				break;
			case 'l':
				l_flag = 1;
				l_arg = optarg;
				break;
			default:
				fprintf(stderr, "incorrect usage: lab4b [--period=#] [--scale=C|F] [log=filename]\n");
				exit(1);
		}
	}

	//set up log file
	if(l_flag && l_arg != NULL){
		logfd = open(l_arg, O_WRONLY | O_APPEND | O_CREAT, 0666);
		if(logfd < 0){
			fprintf(stderr, "error on log file\n");
			exit(2);
		}
	}

	//sensor initialization
	mraa_aio_context sensor;
	sensor = mraa_aio_init(TEMP_PIN);

	if(sensor == NULL){
		mraa_deinit();
		fprintf(stderr, "temperature sensor failed to initialize\n");
		exit(3);
	}

	//button initialization
	mraa_gpio_context button;
	button = mraa_gpio_init(BUTTON_PIN);
	if(button == NULL){
		mraa_deinit();
		fprintf(stderr, "button failed to initialize\n");
		exit(3);
	}

	if(mraa_gpio_dir(button, MRAA_GPIO_IN) != MRAA_SUCCESS){ // set button dir
		fprintf(stderr, "button failed to set direction\n");
		exit(3);
	}

	//get initial temp
	int temp_reading = mraa_aio_read(sensor);
	double temperature = get_temp(temp_reading, s_arg);

	//time variables
	time_t raw_time;
	struct tm last_time, current_time;
	int time_passed = 0; // check if time has passed since last loop

	//start time
	time(& raw_time);
	current_time = * localtime(& raw_time);

	//print first temperature reading
	print_and_log(current_time.tm_hour, current_time.tm_min, current_time.tm_sec, temperature);
	int previous_seconds = current_time.tm_sec;
	last_time = current_time;

	//poll
	struct pollfd poll_arr[1];
	poll_arr[0].fd = 0;
	poll_arr[0].events = POLLIN;

	while(!shutdown_flag){
		poll(poll_arr, 1, 0);

		//update temperature
		temp_reading = mraa_aio_read(sensor);
		temperature = get_temp(temp_reading, s_arg);

		//update time
		time(& raw_time);
		current_time = * localtime(& raw_time);
		int time_now = current_time.tm_min * 60 + current_time.tm_sec;
		int time_prev = last_time.tm_min * 60 + last_time.tm_sec;
		time_passed = time_now - time_prev;

		//print if correct conditions
		int can_print = run_flag && time_passed > 0 && time_passed % p_arg == 0 && current_time.tm_sec != previous_seconds;
		if(can_print){
			print_and_log(current_time.tm_hour, current_time.tm_min, current_time.tm_sec, temperature);
			previous_seconds = current_time.tm_sec;
			last_time = current_time;
		}

		//poll for input
		if((poll_arr[0].revents & POLLIN) == POLLIN){
			//read from STDIN
			read_length = read(STDIN_FILENO, read_buf, 256);

			//loop through each char in the read buffer
			int i = 0;
			int previous_i = 0;
			for(i = 0; i < read_length; i++){
				char * argument_value;

				//find the \n or loop till length
				while(i < read_length && read_buf[i] != '\n'){
					i++;
				}
				//if there is no \n then break
				if(i == read_length){
					break;
				}

				//if found \n
				if(read_buf[i] == '\n'){
					//log everything in read buffer until this point
					if(l_flag){
						int write_length = write(logfd, &read_buf[previous_i], i - previous_i + 1);
						if(write_length < 0){
							fprintf(stderr, "error writing command to log file\n");
							exit(1);
						}
						fsync(logfd);
					}

					//replace \n with end of string
					read_buf[i] = '\0';

					char* argument = read_buf + previous_i;

					int is_scale 	= strncmp(argument, "SCALE=", 	6);
					int is_period 	= strncmp(argument, "PERIOD=", 	7);
					int is_log 		= strncmp(argument, "LOG", 		3);

					int is_stop 	= strcmp(argument, "STOP"		 );
					int is_start 	= strcmp(argument, "START"		 );
					int is_off 		= strcmp(argument, "OFF"		 );

					if(is_scale == 0){
						//get the value
						previous_i += 6;
						argument_value = read_buf + previous_i;
						if(strlen(argument_value) != 1){
							fprintf(stderr, "incorrect use of scale\n");
							exit(1);
						}

						int isC = strcmp(argument_value, "C");
						int isF = strcmp(argument_value, "F");

						if(isC == 0){
							s_arg = 'C';
						}
						else if(isF == 0){
							s_arg = 'F';
						}
						else{
							fprintf(stderr, "incorrect use of scale must be C or F\n");
							exit(1);
						}
					}
					if(is_period == 0){
						//get the value
						previous_i += 7;
						argument_value = read_buf + previous_i;

						int input = atoi(argument_value);
						if(input > 0){
							p_arg = input;
						}
						else{
							fprintf(stderr, "incorrect use of period\n");
							exit(1);
						}
					}
					if(is_log == 0){
						//do nothing
					}
					if(is_stop == 0){
						run_flag = 0;
					}
					if(is_start == 0){
						run_flag = 1;
					}
					if(is_off == 0){
						shutdown_flag = 1;
						break;
					}
					previous_i = i + 1;
				}
			}
		}

		//if button was pressed then stop polling
		if(mraa_gpio_read(button)){
			shutdown_flag = 1;
		}
	}

	//get final time
	time(& raw_time);
	current_time = * localtime(& raw_time);

	//print shutdown
	print_and_log(current_time.tm_hour, current_time.tm_min, current_time.tm_sec, -1.0);

	//close sensor
	if(mraa_aio_close(sensor) != MRAA_SUCCESS){
		fprintf(stderr, "error closing sensor\n");
		exit(3);
	}

	//close button
	if(mraa_gpio_close(button) != MRAA_SUCCESS){
		fprintf(stderr, "error closing button\n");
		exit(3);
	}

	//close log
	if(l_flag){
		close(logfd);
	}
	return 0;
}

double get_temp(int temp_reading, char scale){
	//set up constants for temp reading
	const int B = 4275;		// B value of the thermistor
	const double R0 = 100000;	// R0 = 100k

	//convert to C or F
	double R = (1023.0/ (double) temp_reading) - 1.0;
	R *= R0;

	// convert to temperature via datasheet
	double temperature_c = 1.0 / (log(R / R0) / B + 1.0 / 298.15) - 273.15;

	//convert C to F
	double temperature_f = temperature_c * (9.0/5.0) + 32.0;

	//output one or the other
	if(scale == 'C'){
		return temperature_c;
	}
	else if(scale == 'F'){
		return temperature_f;
	}
	else{
		fprintf(stderr, "invalid use of scale argument\n");
		exit(1);
		return -1;
	}
}

void print_and_log(int hour, int min, int sec, double temperature){
	if(shutdown_flag){
		printf("%02d:%02d:%02d SHUTDOWN\n", hour, min, sec);
		if(l_flag)
			dprintf(logfd, "%02d:%02d:%02d SHUTDOWN\n", hour, min, sec);
	}
	else{
		printf("%02d:%02d:%02d %.1f\n", hour, min, sec, temperature);
		if(l_flag)
			dprintf(logfd, "%02d:%02d:%02d %.1f\n", hour, min, sec, temperature);
	}
}