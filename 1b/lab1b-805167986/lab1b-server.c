// NAME: William Randall
// EMAIL: wrandall1000@gmail.com
// ID: 805167986

#include <fcntl.h> //open //creat
#include <unistd.h> //close //fork //pipe
#include <stdlib.h> //exit
#include <getopt.h> //getopt
#include <stdio.h> //fprintf
#include <termios.h> //termios
#include <poll.h> //poll
#include <errno.h> //errno
#include <signal.h> //signal
#include <string.h> //strerror
#include <zlib.h> //zlib
#include <string.h> //string  //bcopy //atoi
#include <netinet/in.h> //hton
// #include <netdb.h> //gethostbyname //hostent
#include <sys/wait.h> //waitpid //WEXITSTATUS
// #include <sys/types.h>
#include <sys/socket.h> //socket //AF_INET

//GLOBAL VARS
//flags//
int p_flag = 0;
int c_flag = 0;
int s_flag = 0;

//saved args//
int p_arg = -1;
char* program = "/bin/sh"; //the default program

//file descriptors
int sockfd;
int c_sockfd;

//structs
struct sockaddr_in address;
struct sockaddr_in client_address;
struct termios stdin_attr_orig;
struct termios stdin_attr;
static struct option arr_option[] =
{
	{"port",		required_argument,	0,	'p'},
	{"compress",	no_argument,		0,	'c'},
	{"shell",		required_argument,	0,	's'},
	{0, 0, 0, 0}
};

//set up z_stream for compression
z_stream stdin_stream;
z_stream server_stream;

//set up pid
pid_t PID;

//pipes
int pipe_p_c[2];	//pipe from parent to child
int pipe_c_p[2];	//pipe from child to parent
					//{input file descriptor, output file description}

//buffer variables
unsigned char buf[256];	//read input into 256 size buf
unsigned char cbuf[256];	//compressed buffer
int size = 0;
const char crlr[2] = "\r\n";
unsigned char eof_end_string[2] = {0x04, '\0'};

//helper functions
void reset_term();
void set_up_zstream();
void set_up_socket();
void set_up_pipes();
void sig_handler();
void fix_pipes();
void regular_echo();
void child();
void parent();


int main(int argc, char *argv []){
	bzero(cbuf, 256);

	//argument parsing variables
	int ind = 0;
	int ch;

	//argument parsing
	while ( (ch = getopt_long(argc, argv, "p:s:c", arr_option, &ind) ) != -1){
		switch (ch){
			case 'p':
				p_arg = atoi(optarg);
				p_flag = 1;
				break;
			case 'c':
				c_flag = 1;
				break;
			case 's':
				s_flag = 1;
				program = optarg;
				break;
			default:
				fprintf(stderr, "incorrect usage: lab1b-server [--port=port-number] [--compress] [--shell=program]\n");
				exit(1);
		}
	}

	//check to make sure port number was given
	if(p_arg == -1){
		fprintf(stderr, "error port-number not given\n");
		exit(1);
	}

	//set up zstream
	set_up_zstream();
	//set up the socket
	set_up_socket();
	//handle an exit
	atexit(fix_pipes);
	//set up the pipes
	set_up_pipes();
	//set up signal handler
	signal(SIGINT, sig_handler);
	signal(SIGPIPE, sig_handler);

	//fork child process
	if((PID = fork()) < 0){
		fprintf(stderr, "failed to fork: %s\n", strerror(errno));
		exit(1);
	}

	//check if p_flag and if program is not null
	if(p_flag && program != NULL){
		//if child
		if(PID == 0){
			child();
		}
		//if parent
		else{
			parent();
		}
	}
	//echo across socket
	else{
		regular_echo();
	}

	//check if compression then fix streams
	if(c_flag){
		deflateEnd(&server_stream);
		inflateEnd(&stdin_stream);
	}

	//close socket
	shutdown(sockfd, SHUT_RDWR);
	exit(0);
}

//definitions of helper functions
void reset_term(){
	if(tcsetattr(STDIN_FILENO, TCSANOW, &stdin_attr_orig) < 0){
		fprintf(stderr,"cannot restore original attributes for stdin: %s\n", strerror(errno));
		exit(1);
	}
}

void set_up_zstream(){
	//create serer zstream
	server_stream.zalloc 	= Z_NULL;
	server_stream.zfree 	= Z_NULL;
	server_stream.opaque 	= Z_NULL;

	//initialize deflate server zstream with default compression
	if(deflateInit(&server_stream, Z_DEFAULT_COMPRESSION) != Z_OK){
		fprintf(stderr, "error cannot deflate of server zstream\n");
		exit(1);
	}

	//close if no compression
	if(!c_flag){
		deflateEnd(&server_stream);
	}

	//create stdin zstream
	stdin_stream.zalloc 	= Z_NULL;
	stdin_stream.zfree 		= Z_NULL;
	stdin_stream.opaque 	= Z_NULL;

	stdin_stream.avail_in	= 0;
	stdin_stream.next_in	= Z_NULL;

	//initialize inflate server zstream
	if(inflateInit(&stdin_stream) != Z_OK){
		fprintf(stderr, "error cannot inflate of stdin zstream\n");
		exit(1);
	}

	//close if no compression
	if(!c_flag){
		inflateEnd(&stdin_stream);
	}
}

void set_up_socket(){
	//make the socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "error socket could not be opened on server side\n");
		exit(1);
	}

	//set up server address
	bzero((char *) &address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(p_arg);
	address.sin_addr.s_addr = INADDR_ANY; // this maps ip to 127.0.0.1

	//bind server socket
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
		fprintf(stderr, "error with bind on server side\n");
		exit(1);
	}

	//listen on socket for incoming reuqests
	if(listen(sockfd, 5) < 0){
		fprintf(stderr, "error cannot listen\n");
		exit(1);
	}

	socklen_t client_address_length = sizeof(client_address);
	if((c_sockfd = accept(sockfd, (struct sockaddr*) &client_address, &client_address_length)) < 0){
		fprintf(stderr, "error cannot accept the socket connection on server side\n");
		exit(1);
	}
}

void set_up_pipes(){
	//create pipes
	if(pipe(pipe_p_c) < 0){
		fprintf(stderr, "failed to set up pipeline parent to child: %s\n", strerror(errno));
		exit(1);
	}
	if(pipe(pipe_c_p) < 0){
		fprintf(stderr, "failed to set up pipeline child to parent: %s\n", strerror(errno));
		exit(1);
	}
}

void sig_handler(){
	return;
}

void fix_pipes(){
	close(pipe_c_p[0]);
	close(pipe_c_p[1]);
	close(pipe_p_c[0]);
	close(pipe_p_c[1]);
	close(sockfd);
}

void regular_echo(){
	int stop_flag = 0;
	while(!stop_flag){
		if((size = read(c_sockfd, buf, 256)) < 0){
			fprintf(stderr, "error reading from socket\n");
			break;
		}
		int i = 0;
		for(; i < size; i++){
			//^C or ^D
			if(buf[i] == 0x03 || buf[i] == 0x04){
				stop_flag = 1;
				break;
			}
		}
		//if ^C or ^D was entered write remaining then break
		if(stop_flag){
			write(c_sockfd, buf, i + 1);
			break;
		}
		//else write then continue unless error
		else if(write(c_sockfd, buf, size) == EPIPE){
			break;
		}
	}
}

void child(){
	//close extra pipes
	close(pipe_c_p[0]);	//no input for child to parent
	close(pipe_p_c[1]);	//no ouput for parent to child
	
	//terminal -> stdin
	close(STDIN_FILENO);
	dup(pipe_p_c[0]);
	close(pipe_p_c[0]);
	
	//stdout -> terminal
	close(STDOUT_FILENO);
	dup(pipe_c_p[1]);

	//stderr -> terminal
	close(STDERR_FILENO);
	dup(pipe_c_p[1]);
	close(pipe_c_p[1]);

	//run program on child process
	execl(program, program,(char *)NULL);
	fprintf(stderr, "error with child executing program: %s\n", strerror(errno));
	exit(1);
}

void parent(){
	//close extra pipes
	close(pipe_c_p[1]);
	close(pipe_p_c[0]);

	//declare pollfd struct
	struct pollfd poll_in[2]; // {stdin, pipein}
	
	//set up file descriptors
	poll_in[0].fd = c_sockfd;
	poll_in[1].fd = pipe_c_p[0];

	//set up events
	poll_in[0].events = POLLIN;
	poll_in[1].events = POLLIN | POLLERR | POLLHUP;

	//begin poll
	int poll_val = 0;
	int stat;
	while (1){
		//poll the input streams
		if((poll_val = poll(poll_in, 2, -1)) < 0){
			fprintf(stderr, "error with poll for input: %s\n", strerror(errno));
			kill(PID, SIGKILL);
			exit(1);
		}

		//no input was detected with poll
		else if(poll_val == 0){
			continue;
		}
		//an input was detected with poll

		if(poll_in[0].revents & (POLLHUP | POLLERR)){
			waitpid(PID, &stat, 0);
			fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", stat & 0x00ff, WEXITSTATUS(stat));
			break;
		}

		if(poll_in[1].revents & (POLLHUP | POLLERR)){
			close(pipe_p_c[1]);
			break;
		}
		//read from child socket
		if(poll_in[0].revents & POLLIN){
			if((size = read(c_sockfd, buf, 256)) <= 0){
				close(pipe_p_c[1]);
			}

			//if compressed
			if(c_flag && size > 0){
				//set up the variables related to compression
				stdin_stream.avail_in 	= size;
				stdin_stream.next_in 	= buf;
				while(stdin_stream.avail_in > 0){
					stdin_stream.next_out	= cbuf;
					stdin_stream.avail_out	= 256;
					//inflate stdin zstream
					inflate(&stdin_stream, Z_SYNC_FLUSH);
					unsigned int size_compressed_write = 256-stdin_stream.avail_out;

					for(unsigned int i = 0; i < size_compressed_write; i++){
						//^D
						if(cbuf[i] == 0x04){
							close(pipe_p_c[1]);
							break;
						}
						//^C
						else if (cbuf[i] == 0x03){
							kill(PID, SIGINT);
							break;
						}
						else if(cbuf[i] == '\n' || cbuf[i] == '\r'){
							//write a newline
							if(write(pipe_p_c[1], &crlr[1], 1) == EPIPE){
								close(pipe_p_c[1]);
							}
						}
						else{
							if(write(pipe_p_c[1], cbuf + i, 1) == EPIPE){
								close(pipe_p_c[1]);
							}
						}
					}
				}
			}
			//if not compressed
			else{
				for(int i = 0; i < size; i++){
					//^D
					if(buf[i] == 0x04){
						close(pipe_p_c[1]);
						break;
					}
					//^C
					else if(buf[i] == 0x03){
						kill(PID, SIGINT);
						break;
					}
					else if(buf[i] == '\r' || buf[i] == '\n'){
						if(write(pipe_p_c[1], &crlr[1], 1) == EPIPE){
							close(pipe_p_c[1]);
						}
					}
					else{
						if(write(pipe_p_c[1], buf + i, 1) == EPIPE){
							close(pipe_p_c[1]);
						}
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
			//if read is 0 then end the stream
			if(!size){
				if(c_flag){
					server_stream.next_in	= eof_end_string;
					server_stream.avail_in	= 2;
					while(server_stream.avail_in > 0){
						server_stream.avail_out = 256;
						server_stream.next_out = cbuf;
						//deflate server stream
						deflate(&server_stream, Z_SYNC_FLUSH);

						int size_compressed_write = 256-server_stream.avail_out;
						if(write(c_sockfd, cbuf, size_compressed_write) == EPIPE){
							shutdown(c_sockfd, SHUT_RDWR);
						}
					}
				}
				else{
					write(c_sockfd, eof_end_string, 1);
				}
				break;
			}
			//otherwise write normally
			else{
				if(c_flag){
					server_stream.next_in	= buf;
					server_stream.avail_in	= size;
					while(server_stream.avail_in > 0){
						server_stream.avail_out = 256;
						server_stream.next_out = cbuf;
						//deflate server stream
						deflate(&server_stream, Z_SYNC_FLUSH);

						int size_compressed_write = 256-server_stream.avail_out;
						write(c_sockfd, cbuf, size_compressed_write);
					}
				}
				else{
					write(c_sockfd, buf, size);
				}
			}
		}
	}

	//catch child exit stat
	waitpid(PID, &stat, 0);
	if(WIFSIGNALED(stat) || WIFEXITED(stat)){
		fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\r\n", stat & 0x00ff, WEXITSTATUS(stat));
		return;
	}
	else{
		fprintf(stderr, "error the child process exited not normally\r\n");
		return;
	}
}