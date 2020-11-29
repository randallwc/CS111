# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra -lmraa -lm
NAME=lab4b
UID=805167986
FILES=README Makefile lab4b.c lab_test.sh

default: $(NAME).c
	$(CC) $(CFLAGS) -o $(NAME) $(NAME).c

check: default lab_test.sh
	bash lab_test.sh

clean:
	rm -f $(NAME)
	rm -f $(NAME)-$(UID).tar.gz

dist:
	tar -czf $(NAME)-$(UID).tar.gz $(FILES)