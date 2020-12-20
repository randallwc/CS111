# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra -lmraa -lm
NAME=lab4c
UID=805167986
FILES=README Makefile lab4c_tcp.c lab4c_tls.c

default: tcp tls

tcp: $(NAME)_tcp.c
	$(CC) $(CFLAGS) -o $(NAME)_tcp $(NAME)_tcp.c

tls: $(NAME)_tls.c
	$(CC) $(CFLAGS) -o $(NAME)_tls $(NAME)_tls.c -lssl -lcrypto

clean:
	rm -f $(NAME)_tcp
	rm -f $(NAME)_tls
	rm -f $(NAME)-$(UID).tar.gz

dist:
	tar -czf $(NAME)-$(UID).tar.gz $(FILES)