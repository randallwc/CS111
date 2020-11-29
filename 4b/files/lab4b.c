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

//set up defines for pins
#define TEMP_PIN 1
#define BUTTON_PIN 60

//set up constants for temp reading
const int B = 4275;		// B value of the thermistor
const int R0 = 100000;	// R0 = 100k

//ex output:
//"17:25:58 98.6\n"
//write to fd 1

//
//--period=#				//default 1(seconds)
//--scale=C (or --scale=F)	//default F
//log=filename				//optional

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


// int main(int argc, char ** argv){
int main(){
	//set up sensors
	mraa_aio_context sensor;
	sensor = mraa_aio_init(TEMP_PIN);
	mraa_gpio_dir(sensor, MRAA_GPIO_IN);

	printf("1\n");

	mraa_gpio_context button;
	button = mraa_gpio_init(BUTTON_PIN);
	mraa_gpio_dir(button, MRAA_GPIO_IN);

	printf("1.1\n");

	//read
	int temp_reading = mraa_aio_read(sensor);

	printf("2\n");

	float R = 1023.0/temp_reading-1.0;
	R *= R0;
	float temperature_c = 1.0/(log(R/R0)/B+1.0/298.15)-273.15; // convert to temperature via datasheet
	float temperature_f = temperature_c * (9.0/5.0) + 32.0;	//convert C to F

	printf("3\n");

	printf("temperature = %f %f", temperature_c, temperature_f);

	//close sensors
	mraa_aio_close(sensor);
	return 0;
}

