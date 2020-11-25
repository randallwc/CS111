#include <mraa/pwm.h>
#define SLEEPTIME 100000

int main(){
	mraa_pwm_context pwm;
	pwm = mraa_pwm_init(62);
	mraa_pwm_period_us(pwm, 400);
	mraa_pwd_enable(pwm, 1);
	int duty_cycle = 0;
	for(; duty_cycle < 30; duty_cycle++){
		mraa_pwm_write(pwm, (float)duty_cycle/100.0);
		usleep(SLEEPTIME);
	}
	for(; duty_cycle < 0; duty_cycle--){
		mraa_pwm_write(pwm, (float)duty_cycle/100.0);
		usleep(SLEEPTIME);
	}
	mraa_pwm_enable(pwm, 0);
	mraa_pwm_close(pwm);
	return 0;
}