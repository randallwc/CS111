# NAME: William Randall
# EMAIL: wrandall1000@gmail.com
# ID: 805167986

CC=gcc
CFLAGS=-Wall -Wextra

lab0: lab0.c
	@$(CC) lab0.c -o lab0 $(CFLAGS)

check: lab0
	# make and fill in files
	@echo -e 'test\ntest\ntest\ntest\ntest' > IN
	@chmod -w IN
	@echo "" > OUT
	@echo -e 'bad\nbad\nbad\nbad\nbad' > BAD
	@echo "" > DIFF

	# remove perms for BAD
	@chmod -rwx BAD

	# CHECK NORMAL CASES
	# file to file
	@./lab0 --input=IN --output=OUT
	@diff IN OUT > DIFF
	@cat DIFF
	@echo "" > OUT
	@echo "" > DIFF
	# stdin to file
	@./lab0 --output=OUT < IN
	@diff IN OUT > DIFF
	@cat DIFF
	@echo "" > OUT
	@echo "" > DIFF
	# file to stdout
	@./lab0 --input=IN > OUT
	@diff IN OUT > DIFF
	@cat DIFF
	@echo "" > OUT
	@echo "" > DIFF
	# stdin to stdout
	@./lab0 < IN > OUT
	@diff IN OUT > DIFF
	@cat DIFF
	@echo "" > OUT
	@echo "" > DIFF

	# CHECK BAD CASES
	# unrecognized argument
	-./lab0 --wrong_arg
	# unable to open input file
	-./lab0 --input=BAD
	# unable to open output file
	-./lab0 --output=BAD
	# caught and recieved SIGSEGV
	-./lab0 --catch --segfault
	-./lab0 --segfault --catch


	@rm -f IN
	@rm -f OUT
	@rm -f BAD
	@rm -f DIFF

clean:
	@rm -f ./lab0
	@rm -f *.tar.gz
	@rm -f IN
	@rm -f OUT
	@rm -f BAD
	@rm -f DIFF

dist: lab0.c README backtrace.png breakpoint.png Makefile
	@tar -czf lab0-805167986.tar.gz lab0.c README backtrace.png breakpoint.png Makefile

