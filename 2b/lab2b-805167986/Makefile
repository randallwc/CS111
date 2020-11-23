# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra -lpthread -g
NAME=lab2b
UID=805167986

FILES=SortedList.h SortedList.c lab2_list.c Makefile README list_test.sh lab2_list.gp lab2b_list.csv profile.out *.png

.SILENT:default
.SILENT:tests
.SILENT:profile
.SILENT:graphs
.SILENT:dist
.SILENT:clean

default: lab2_list.c SortedList.c SortedList.h
	$(CC) $(CFLAGS) -o lab2_list lab2_list.c SortedList.c

tests: default list_test.sh
	chmod +x list_test.sh
	./list_test.sh > lab2b_list.csv

profile: default
	rm -f raw.gperf
	LD_PRELOAD=/usr/lib64/libprofiler.so \
	CPUPROFILE=raw.gperf ./lab2_list --iterations=1000 --threads=12 --sync=s
	pprof --text ./lab2_list raw.gperf > profile.out
	pprof --list=thread ./lab2_list raw.gperf >> profile.out
	rm -f raw.gperf

graphs: tests lab2_list.gp
	chmod +x lab2_list.gp
	./lab2_list.gp
	
dist: graphs profile
	tar -cvzf $(NAME)-$(UID).tar.gz $(FILES)

clean:
	rm -f lab2_list
	rm -f $(NAME)-$(UID).tar.gz
