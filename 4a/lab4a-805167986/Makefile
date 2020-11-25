# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra
NAME=lab4a
UID=805167986

.SILENT:default
.SILENT:clean
.SILENT:dist

default:
	$(CC) $(CFLAGS) -o $(NAME) $(NAME).c

clean:
	rm -f $(NAME)
	rm -f $(NAME)-$(UID).tar.gz

dist:
	tar -czf $(NAME)-$(UID).tar.gz README Makefile my_beaglebone.jpg connected.png copy.png build_run.png git.png