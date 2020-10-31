# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra
NAME=lab1a
UID=805167986

.SILENT: default
.SILENT: clean
.SILENT: dist

default: $(NAME).c
	$(CC) $(NAME).c -o $(NAME) $(CFLAGS)

clean:
	rm -f $(NAME)
	rm -f $(NAME)-$(UID).tar.gz

dist: lab1a.c README Makefile
	tar -czf $(NAME)-$(UID).tar.gz $(NAME).c README Makefile

