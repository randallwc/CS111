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
#include <math.h> //

//set up constants for pins
const int TEMP_PIN = 1;
const int BUTTON_PIN  = 60;

//set up constants for temp reading
const int B = 4275;		// B value of the thermistor
const int R0 = 100000;	// R0 = 100k

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

//argument variables
int p_arg 		= 1;
char s_arg		= 'F';
char * l_arg	= NULL;
int logfd 		= -1;

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
		if((logfd = (l_arg, O_WRONLY | O_APPEND | O_CREAT, 0666)) < 0){
			fprintf(stderr, "error on log file\n");
			exit(2);
		}
	}

	//set up sensor
	mraa_aio_context sensor = mraa_aio_init(TEMP_PIN);
	mraa_gpio_dir(sensor, MRAA_GPIO_IN);

	//set up button
	mraa_gpio_context button = mraa_gpio_init(BUTTON_PIN);
	mraa_gpio_dir(button, MRAA_GPIO_IN);




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
	return 0;
}

{
	//read
	int temp_reading = mraa_aio_read(sensor);

	float R = 1023.0/temp_reading-1.0;
	R *= R0;
	float temperature_c = 1.0/(log(R/R0)/B+1.0/298.15)-273.15; // convert to temperature via datasheet
	float temperature_f = temperature_c * (9.0/5.0) + 32.0;	//convert C to F
}

