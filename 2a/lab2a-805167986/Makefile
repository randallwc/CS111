# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra -lpthread
NAME=lab2a
UID=805167986

FILES=lab2_add.c SortedList.h SortedList.c lab2_list.c Makefile README add_test.sh list_test.sh *.csv *.png *.gp

default: lab2_add.c lab2_list.c SortedList.c
	$(CC) $(CFLAGS) -o lab2_add lab2_add.c
	$(CC) $(CFLAGS) -o lab2_list -g lab2_list.c SortedList.c

tests: default add_test.sh list_test.sh
	chmod +x add_test.sh
	chmod +x list_test.sh
	./add_test.sh > lab2_add.csv
	./list_test.sh > lab2_list.csv

graphs: lab2_list.gp lab2_add.gp tests
	./lab2_add.gp
	./lab2_list.gp
	
dist: graphs
	tar -cvzf $(NAME)-$(UID).tar.gz $(FILES)

clean:
	rm -f lab2_add
	rm -f lab2_list
	rm -f $(NAME)-$(UID).tar.gz
