// NAME: William Randall
// EMAIL: wrandall1000@gmail.com
// ID: 805167986
// lab4c_tcp.c

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

// #include <sys/types.h>
#include <sys/socket.h> //socket
#include <netdb.h> //sockaddr_in//htons

//set up constants for pins
#define TEMP_PIN 1
// #define BUTTON_PIN 60
//set up constants for server
int port = 18000;

//GLOBAL STRUCT
static struct option arr_option[] =
{
    {"period",  required_argument,      0,  'p'},
    {"scale",   required_argument,      0,  's'},
    {"log",     required_argument,      0,  'l'},
    {"id",      required_argument,      0,  'i'},
    {"host",    required_argument,      0,  'h'},
    {0, 0, 0, 0}
};
//--period=#                //default 1(seconds)
//--scale=C (or --scale=F)  //default F
//log=filename              //optional

//flag variables
int p_flag = 0;
int s_flag = 0;
int l_flag = 0;
int i_flag = 0;
int h_flag = 0;
int run_flag = 1;
int shutdown_flag = 0;
int SHUTDOWN_PRINTED = 0;

//argument variables
int p_arg       = 1;
char s_arg      = 'F';
char * l_arg    = NULL;
int logfd       = -1;
char * h_arg    = NULL;
int i_arg       = 111111111;


//buffers
char read_buf[256] = {0};
char send_buf[32];
int read_length = 0;

//socket
int sockfd = -1;

//helper functions
double get_temp(int temp_reading, char scale);
void print_and_log(int hour, int min, int sec, double temperature);
void do_when_interrupted();
void check_id(int val);

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
//write to socket

int main(int argc, char ** argv){

    int ind = 0;
    int ch = 0;
    //argument parsing
    while ( (ch = getopt_long(argc, argv, "p:s:l:i:h:", arr_option, &ind) ) != -1){
        switch (ch){
            case 'p':
                p_flag = 1;
                p_arg = atoi(optarg);
                if(p_arg <= 0){
                    fprintf(stderr, "period must be greater than 0\n");
                    exit(1);
                }
                break;
            case 's':
                s_flag = 1;
                //check if len==1
                if(strlen(optarg) != 1){
                    fprintf(stderr, "incorrect usage: lab4b [--period=#] [--scale=C|F] [--log=filename] [--id=9-digit-number] [--host=name or address]\n");
                    exit(1);
                }
                //check if F or C
                if(optarg[0] == 'F'){
                    s_arg = *optarg;
                }
                else if(optarg[0] == 'C'){
                    s_arg = *optarg;
                }
                else{
                    fprintf(stderr, "incorrect usage: lab4b [--period=#] [--scale=C|F] [--log=filename] [--id=9-digit-number] [--host=name or address]\n");
                    exit(1);
                }
                break;
            case 'l':
                l_flag = 1;
                l_arg = optarg;
                if(l_arg == NULL){
                    fprintf(stderr, "invalid log file\n");
                    exit(1);
                }
                break;
            case 'h':
                h_flag = 1;
                h_arg = optarg;
                if(h_arg == NULL){
                    fprintf(stderr, "invalid host argument\n");
                    exit(1);
                }
                break;
            case 'i':
                i_flag = 1;
                i_arg = atoi(optarg);
                check_id(i_arg);
                break;
            default:
                fprintf(stderr, "incorrect usage: lab4b [--period=#] [--scale=C|F] [--log=filename] [--id=9-digit-number] [--host=name or address]\n");
                exit(1);
        }
    }

    //set up port number
    if(optind < argc){
        port = atoi(argv[optind]);
    }
    else{
        fprintf(stderr, "no port number: incorrect usage: lab4b [--period=#] [--scale=C|F] [--log=filename] [--id=9-digit-number] [--host=name or address]\n");
        exit(1);
    }

    //set up log file
    if(l_flag && l_arg != NULL){
        logfd = open(l_arg, O_WRONLY | O_APPEND | O_CREAT, 0666);
        if(logfd < 0){
            fprintf(stderr, "error on log file\n");
            exit(2);
        }
    }

    //make socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        fprintf(stderr, "error creating socket\n");
        exit(2);
    }

    //get the socket address
    struct sockaddr_in socket_address;
    bzero((char *) &socket_address, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);

    //get the server ip address
    struct hostent * server_ip_address = gethostbyname(h_arg);
    //copy over the s_addr from the server entity
    memcpy(
        (char *)(&socket_address.sin_addr.s_addr), 
        (char *)(server_ip_address->h_addr), 
        server_ip_address->h_length
    );

    //connect to server
    if (connect(sockfd, (struct sockaddr *)&socket_address, sizeof(socket_address)) == -1){
        fprintf(stderr, "error with connect\n");
        exit(2);
    }

    // //send the id
    // dprintf(sockfd, "ID=%i\n", i_arg);
    // if(l_flag){
    //     int return_value_dpf = dprintf(logfd, "ID=%i\n", i_arg);
    //     if(return_value_dpf < 0){
    //         fprintf(stderr, "error printing to log\n");
    //         exit(2);
    //     }
    // }

    //send the id
    if (sprintf(send_buf, "ID=%d\n", i_arg) < 0){
        fprintf(stderr, "error storing string in buffer\n");
        exit(1);
    }
    if (write(sockfd, send_buf, strlen(send_buf)) < 0){
        fprintf(stderr, "error printing id\n");
        exit(1);
    }
    if(l_flag){
        int return_value_dpf = write(logfd, send_buf, strlen(send_buf));
        if(return_value_dpf < 0){
            fprintf(stderr, "error printing to log\n");
            exit(1);
        }
    }

    //sensor initialization
    mraa_aio_context sensor;
    sensor = mraa_aio_init(TEMP_PIN);

    if(sensor == NULL){
        mraa_deinit();
        fprintf(stderr, "temperature sensor failed to initialize\n");
        exit(2);
    }

    //get initial temp
    int temp_reading = mraa_aio_read(sensor);
    double temperature = get_temp(temp_reading, s_arg);

    //time variables
    struct tm current_time;
    time_t curr_t, last_t;

    //start time
    time(& curr_t);
    current_time = * localtime(& curr_t);

    //print first temperature reading
    print_and_log(current_time.tm_hour, current_time.tm_min, current_time.tm_sec, temperature);
    last_t = curr_t;

    //poll
    struct pollfd poll_sock;
    poll_sock.fd = sockfd;
    poll_sock.events = POLLIN;
    poll_sock.revents = 0;

    while(!shutdown_flag){
        //update temperature
        temp_reading = mraa_aio_read(sensor);
        temperature = get_temp(temp_reading, s_arg);

        //update time
        time(& curr_t);
        current_time = * localtime(& curr_t);

        //print if correct conditions
        /**
        run_flag is true
        the difference between the current time and the last time is greater than the period
        */
        int can_print = run_flag && (curr_t - last_t) >= p_arg;

        if(can_print){
            print_and_log(current_time.tm_hour, current_time.tm_min, current_time.tm_sec, temperature);
            last_t = curr_t;
        }

        //poll for input
        int poll_val = poll(&poll_sock, 1, 0);

        if(poll_val != 1){
            continue;
        }

        if((poll_sock.revents & POLLERR) == POLLERR){
            fprintf(stderr, "error when polling\n");
            exit(2);
        }
        
        if((poll_sock.revents & POLLIN) == POLLIN){
            //read from socket
            read_length = read(sockfd, read_buf, 256);

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
                            exit(2);
                        }
                        fsync(logfd);
                    }

                    //replace \n with end of string
                    read_buf[i] = '\0';

                    char* argument = read_buf + previous_i;

                    int is_scale    = strncmp(argument, "SCALE=", 6);
                    int is_period   = strncmp(argument, "PERIOD=", 7);
                    int is_log      = strncmp(argument, "LOG", 3);

                    int is_stop     = strcmp(argument, "STOP");
                    int is_start    = strcmp(argument, "START");
                    int is_off      = strcmp(argument, "OFF");

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
                        if(input <= 0){
                            fprintf(stderr, "incorrect use of period must be greater than 0\n");
                            exit(1);
                        }
                        p_arg = input;
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
    }

    //get final time
    time(& curr_t);
    current_time = * localtime(& curr_t);

    //print shutdown if not already printed
    print_and_log(current_time.tm_hour, current_time.tm_min, current_time.tm_sec, -1.0);

    //close sensor
    if(mraa_aio_close(sensor) != MRAA_SUCCESS){
        fprintf(stderr, "error closing sensor\n");
        exit(2);
    }

    //close log
    if(l_flag){
        close(logfd);
    }

    //close socket
    close(sockfd);
    return 0;
}

double get_temp(int temp_reading, char scale){
    //set up constants for temp reading
    const int B = 4275;     // B value of the thermistor
    const double R0 = 100000;   // R0 = 100k

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
    char logBuffer[32];
    int return_value_pf = -1;
    int return_value_dpf = -1;
    int return_value_spf = -1;
    int entered = 0;

    //print shutdown
    if(shutdown_flag && !SHUTDOWN_PRINTED){
        return_value_spf = sprintf(logBuffer, "%02d:%02d:%02d SHUTDOWN\n", hour, min, sec);
        return_value_pf = write(sockfd,logBuffer, strlen(logBuffer));
        if(l_flag)
            return_value_dpf = write(logfd,logBuffer, strlen(logBuffer));
        SHUTDOWN_PRINTED = 1;
        entered = 1;
    }

    //print normally
    else if(!SHUTDOWN_PRINTED){
        return_value_spf = sprintf(logBuffer, "%02d:%02d:%02d %.1f\n", hour, min, sec, temperature);
        return_value_pf = write(sockfd,logBuffer, strlen(logBuffer));
        if(l_flag)
            return_value_dpf = write(logfd,logBuffer, strlen(logBuffer));
        entered = 1;
    }

    //check for errors
    if(return_value_spf < 0 && entered){
        fprintf(stderr, "error storing string in buffer\n");
        exit(2);
    }
    if(return_value_pf < 0 && entered){
        fprintf(stderr, "error printing to socket\n");
        exit(2);
    }

    if(l_flag && return_value_dpf < 0 && entered){
        fprintf(stderr, "error printing to log\n");
        exit(2);
    }
}

void do_when_interrupted(){
    shutdown_flag = 1;
}

void check_id(int val){
    int number = 0;
    while(val > 0){
        val /= 10;
        number++;
    }
    if(number == 9){
        return;
    }
    else{
        fprintf(stderr, "id is not 9 digits\n");
        exit(1);
    }
}
