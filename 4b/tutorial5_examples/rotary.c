#include <signal.h>
#include <mraa/aio.h>
#include <unistd.h>
#include <stdio.h>

#define SLEEP_TIME 100000

sig_atomic_t volatile run_flag = 1;

void do_when_interrupted(int sig){
	if(sig == SIGINT){
		printf("SIGINT caught\n");
		run_flag = 0;
	}
}

int main() {
	uint16_t value;
	mraa_aio_context rotary;
	rotary = mraa_aio_init(1);
	printf("start\n");
	signal(SIGINT, do_when_interrupted);
	while(run_flag){
		value = mraa_aio_read(rotary);
		printf("value: %d\n", value);
		usleep(SLEEP_TIME);
	}
	printf("shutdown\n");
	mraa_aio_close(rotary);
	return 0;
}