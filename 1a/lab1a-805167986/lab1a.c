// NAME: William Randall
// EMAIL: wrandall1000@gmail.com
// ID: 805167986

#include <fcntl.h> //open & creat
#include <unistd.h> //close // fork pipe
#include <stdlib.h> //exit
#include <getopt.h> //getopt
#include <stdio.h> //fprintf
#include <termios.h> //termios
#include <poll.h> //poll
#include <errno.h> //errno
#include <signal.h> //signal
#include <string.h> //strerror
#include <sys/wait.h> //waitpid

//GLOBAL VARS
//flags//
int s_flag = 0;
int d_flag = 0;
int exit_flag = 0;
//structs
struct termios stdin_attr_orig;
struct termios stdin_attr;
static struct option arr_option[] =
{
	{"shell",	required_argument,	0,	's'},
	{"debug",	no_argument,		0,	'd'},
	{0, 0, 0, 0}
};

//helper functions
void reset_term(){
	if(tcsetattr(STDIN_FILENO, TCSANOW, &stdin_attr_orig) < 0){
		fprintf(stderr,"cannot restore original attributes for stdin: %s\n", strerror(errno));
		exit(1);
	}
}

void sigpipe_handler(int signum){
	if(signum == SIGPIPE){
		exit_flag = 1;
		if(d_flag){
			fprintf(stderr, "sigpipe caught\n");
		}
	}
}

int main (int argc, char *argv []){
	//pid of child
	pid_t childID;
	
	//pipes
	int pipe_p_c[2];	//pipe from parent to child
	int pipe_c_p[2];	//pipe from child to parent
						//{input file descriptor, output file description}

	//buffer variables
	char buf[256];	//read input into 256 size buf
	int size = 0;
	const char crlr[2] = "\r\n";
	const char eof[2] = "^D";
	const char inter[2] = "^C";

	//argument parsing variables
	int ind = 0;
	int ch;
	char* program;

	//argument parsing
	while ( (ch = getopt_long(argc, argv, "s:d", arr_option, &ind) ) != -1){
		switch (ch){
			case 's':
				s_flag = 1;
				program = optarg;
				break;
			case 'd':
				d_flag = 1;
				break;
			default:
				fprintf(stderr, "incorrect usage: lab1 [--shell=program]\n");
				exit(1);
		}
	}

	//get attributes for stdin
	if(tcgetattr(STDIN_FILENO, &stdin_attr) < 0){
		fprintf(stderr,"cannot get attributes for stdin: %s\n", strerror(errno));
		exit(1);
	}

	//make a copy
	stdin_attr_orig = stdin_attr;

	//change attributes for stdin
	stdin_attr.c_iflag = ISTRIP;	/* only lower 7 bits*/
	stdin_attr.c_oflag = 0;			/* no processing	*/
	stdin_attr.c_lflag = 0;			/* no processing	*/

	//set new attributes
	if(tcsetattr(STDIN_FILENO, TCSANOW, &stdin_attr) < 0){
		fprintf(stderr,"cannot set new attributes for stdin: %s\n", strerror(errno));
		exit(1);
	}

	//when program ends reset terminal
	atexit(reset_term);

	//if --shell flag then set up pipelines
	if(s_flag){
		//set up signal handler
		signal(SIGPIPE, sigpipe_handler);

		//create pipes
		if(pipe(pipe_p_c) < 0){
			fprintf(stderr, "failed to set up pipeline parent to child: %s\n", strerror(errno));
			exit(1);
		}
		if(pipe(pipe_c_p) < 0){
			fprintf(stderr, "failed to set up pipeline child to parent: %s\n", strerror(errno));
			exit(1);
		}

		//fork the child process
		if((childID = fork()) < 0){
			fprintf(stderr, "failed to fork: %s\n", strerror(errno));
			exit(1);
		}
		//do tasks as the child process
		else if(childID == 0){
			//close extra pipes
			close(pipe_c_p[0]);	//no input for child to parent
			close(pipe_p_c[1]);	//no ouput for parent to child
			
			//terminal -> stdin
			close(STDIN_FILENO);
			dup(pipe_p_c[0]);
			close(pipe_p_c[0]);
			
			//stderr & stdout -> terminal
			close(STDOUT_FILENO);
			dup(pipe_c_p[1]);

			close(STDERR_FILENO);
			dup(pipe_c_p[1]);

			close(pipe_c_p[1]);

			//run program on child process
			execl(program, program,(char *)NULL);
			fprintf(stderr, "error with child executing program: %s\n", strerror(errno));
			exit(1);
		}//end child

		//do tasks as the parent process
		else{
			//close extra pipes
			close(pipe_c_p[1]);
			close(pipe_p_c[0]);

			//declare pollfd struct
			struct pollfd poll_in[2]; // {stdin, pipein}
			
			//set up file descriptors
			poll_in[0].fd = STDIN_FILENO;
			poll_in[1].fd = pipe_c_p[0];

			//set up events
			poll_in[0].events = POLLIN;
			poll_in[1].events = POLLIN;

			//begin poll
			int stop_flag = 0;
			int poll_val = 0;
			int pipe_close_flag = 0;
			while (!stop_flag){
				//poll the input streams
				if((poll_val = poll(poll_in, 2, -1)) < 0){
					fprintf(stderr, "error with poll for input: %s\n", strerror(errno));
					exit (1);
				}
				//an input was detected with poll
				else if(poll_val > 0){
					if(poll_in[0].revents & (POLLHUP | POLLERR)){
						if(d_flag){
							fprintf(stderr, "stdin POLLHUP or POLLERR\n");
						}
						stop_flag = 1;
					}
					if(poll_in[1].revents & (POLLHUP | POLLERR)){
						if(d_flag){
							fprintf(stderr, "program POLLHUP or POLLERR\n");
						}
						stop_flag = 1;
					}
					//read from stdin
					if(poll_in[0].revents & POLLIN){
						if((size = read(STDIN_FILENO, buf, 256)) < 0){
							fprintf(stderr, "error cannot read from terminal: %s\n", strerror(errno));
							exit(1);
						}
						for(int i = 0; i < size; i++){
							//end of file
							//^D
							if(buf[i] == 0x04){
								if(d_flag){
									fprintf(stderr, "eof stdin\n");
								}
								if(write(STDOUT_FILENO, eof, 2) < 0){
									fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
									exit(1);
								}
								//if pipe is not already closed
								if(pipe_close_flag == 0){
									close(pipe_p_c[1]);
									pipe_close_flag = 1;
								}
							}

							//interrupt
							//^C
							else if(buf[i] == 0x03){
								if(d_flag){
									fprintf(stderr, "interrupt stdin\n");
								}
								if(write(STDOUT_FILENO, inter, 2) < 0){
									fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
									exit(1);
								}
								if(kill(childID, SIGINT) < 0){
									fprintf(stderr, "error sending SIGINT to program: %s\n", strerror(errno));
									exit(1);
								}
							}

							//newline or carriage return
							else if(buf[i] == '\n' || buf[i] == '\r'){
								//both '\r' and '\n' to stdout
								if(write(STDOUT_FILENO, crlr, 2) < 0){
									fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
									exit(1);
								}
								//just '\n' to pipe
								if(write(pipe_p_c[1], &crlr[1], 1) < 0){
									fprintf(stderr, "error writing to pipe: %s\n", strerror(errno));
									exit(1);
								}
							}

							//write 1 char at a time normally
							else {
								if(write(STDOUT_FILENO, buf + i, 1) < 0){
									fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
									exit(1);
								}
								if(write(pipe_p_c[1], buf + i, 1) < 0){
									fprintf(stderr, "error writing to pipe: %s\n", strerror(errno));
									exit(1);
								}
							}
						}
					}
					//read from pipein child to parent
					if(poll_in[1].revents & POLLIN){
						if((size = read(pipe_c_p[0], buf, 256)) < 0){
							fprintf(stderr, "error cannot read from program: %s\n", strerror(errno));
							exit(1);
						}
						for(int i = 0; i < size; i++){
							//end of file
							//^D
							if(buf[i] == 0x04){
								if(d_flag){
									fprintf(stderr, "eof from program\n");
								}
								stop_flag = 1;
								if(write(STDOUT_FILENO, inter, 2) < 0){
									fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
									exit(1);
								}
							}

							//newline
							else if(buf[i] == '\n'){
								if(write(STDOUT_FILENO, crlr, 2) < 0){
									fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
									exit(1);
								}
							}

							//write 1 char at a time normally
							else {
								if(write(STDOUT_FILENO, buf + i, 1) < 0){
									fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
									exit(1);
								}
							}
						}
					}
				}
			}
			//end poll

			//close pipe if not closed
			if(pipe_close_flag == 0){
				close(pipe_p_c[1]);
			}

			//check if the child is waiting
			int child_process_stat;
			if(waitpid(childID, &child_process_stat,0) < 0){
				fprintf(stderr, "error the child process is waiting: %s\n", strerror(errno));
				exit(1);
			}

			//show exit status of child process
			int shell_exit_signal = child_process_stat & 0x00ff;
			int status_v = WEXITSTATUS(child_process_stat);
			fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", shell_exit_signal, status_v);
			exit(0);
		}//end of parent
	}
	//do regular read if no '--shell' option
	else{
		while ((size = read(STDIN_FILENO, buf, 256)) > 0){
			for(int i = 0; i < size; i++){
				//eof stop execution
				//^D
				if(buf[i] == 0x04){
					if(write(STDOUT_FILENO, eof, 2) < 0){
						fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
						exit(1);
					}
					exit(0);
				}
				else if(buf[i] == '\n' || buf[i] == '\r'){
					if(write(STDOUT_FILENO, crlr, 2) < 0){
						fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
						exit(1);
					}
				}
				else {
					if(write(STDOUT_FILENO, buf + i, 1) < 0){
						fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
						exit(1);
					}
				}
			}
		}
		if(size < 0){
			fprintf(stderr, "error reading from stdin: %s\n", strerror(errno));
			exit(1);
		}
	}
	exit(0);
}