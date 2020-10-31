# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra -lz
NAME=lab1b
UID=805167986

.SILENT:default
.SILENT:client
.SILENT:server
.SILENT:clean
.SILENT:dist

default: client server

client:
	$(CC) $(CFLAGS) -o $(NAME)-client $(NAME)-client.c

server:
	$(CC) $(CFLAGS) -o $(NAME)-server $(NAME)-server.c

clean:
	rm -f $(NAME)-client
	rm -f $(NAME)-server
	rm -f *.o
	rm -f $(NAME)-$(UID).tar.gz

dist: default
	tar -czf $(NAME)-$(UID).tar.gz $(NAME)-client.c $(NAME)-server.c README Makefile

