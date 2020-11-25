#include <signal.h>
#include <mraa/gpio.h>
#include <unistd.h>

sig_atomic_t volatile run_flag = 1;

void do_when_interrupted(){
	printf("button pressed\n");
	run_flag = 0;
}

int main() {
	mraa_gpio_context buzzer, button;
	buzzer = mraa_gpio_init(62);
	button = mraa_gpio_init(60);
	mraa_gpio_dir(buzzer, MRAA_GPIO_OUT);
	mraa_gpio_dir(button, MRAA_GPIO_IN);
	mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &do_when_interrupted, NULL);
	while(run_flag){
		mraa_gpio_write(buzzer, 1);
		printf("buzzer high\n");
		sleep(1);
		mraa_gpio_write(buzzer, 0);
		printf("buzzer low\n");
		sleep(1);
	}
	mraa_gpio_write(buzzer, 0);
	printf("buzzer low\n");
	mraa_gpio_close(button);
	mraa_gpio_close(buzzer);
	return 0;
}