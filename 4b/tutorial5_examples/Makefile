CC=gcc
CFLAG=-lmraa -Wextra -Wall

version:
	$(CC) $(CFLAG) -o check_mraa_version check_mraa_version.c
	
buzzer:
	$(CC) $(CFLAG) -o buzzer buzzer.c

button:
	$(CC) $(CFLAG) -o gpio_interrupt gpio_interrupt.c

rotary:
	$(CC) $(CFLAG) -o rotary rotary.c

volume:
	$(CC) $(CFLAG) -o buzzer_volume buzzer_volume.c