# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra -g -lpthread
NAME=lab2a
UID=805167986

ADD_PNG=lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png
LIST_PNG=lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png
CSV_FILES=lab2_add.csv lab2_list.csv
FILES=lab2_add.c SortedList.h SortedList.c lab2_list.c Makefile $(CSV_FILES) $(ADD_PNG) $(LIST_PNG) README add_test.sh list_test.sh

# .SILENT:default

build: default
default: add list
add: lab2_add.c
	$(CC) $(CFLAGS) -o lab2_add lab2_add.c

list: lab2_list.c SortedList.c
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c

# put a '-' in front of test cases that will fail
tests: lab2_add lab2_list add_test.sh list_test.sh
	chmod +x add_test
	chmod +x list_test
	./add_test.sh > lab2_add.csv
	./list_test.sh > lab2_list.csv

graphs: tests lab2_add.gp lab2_list.gp
	./lab2_add.gp
	./lab2_list.gp

dist: build tests graphs
	tar -czf $(NAME)-$(UID).tar.gz $(FILES)

clean:
	rm -f lab2_add
	rm -f lab2_list
	rm -f $(NAME)-$(UID).tar.gz
	