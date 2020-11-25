CC=gcc
CFLAG=-lmraa -Wextra -Wall

version:
	$(CC) $(CFLAG) -o check_mraa_version check_mraa_version.c
	
buzzer:
	$(CC) $(CFLAG) -o buzzer buzzer.c

button:
	$(CC) $(CFLAG) -o gpio_interrupt gpio_interrupt.c

potentiometer:
	$(CC) $(CFLAG) -o rotary rotary.c