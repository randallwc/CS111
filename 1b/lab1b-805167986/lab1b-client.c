// NAME: William Randall
// EMAIL: wrandall1000@gmail.com
// ID: 805167986

#include <fcntl.h> //open //creat
// #include <unistd.h> //close //fork //pipe
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
#include <netdb.h> //gethostbyname //hostent
// #include <ulimit.h> 
#include <sys/wait.h> //waitpid //WEXITSTATUS
// #include <sys/types.h>
#include <sys/socket.h> //socket //AF_INET //shutdown
#include <sys/time.h> //rlimit
#include <sys/resource.h> //rlimit

//GLOBAL VARS
//flags//
int p_flag = 0;
int l_flag = 0;
int c_flag = 0;

//saved args//
int p_arg = -1;
char* l_arg = NULL;

//file descriptors
int logfd = -1;
int sockfd;

//structs
struct sockaddr_in address;
// struct hostent *server_ent;
struct termios stdin_attr_orig;
struct termios stdin_attr;
static struct option arr_option[] =
{
	{"port",		required_argument,		0,	'p'},
	{"log",			required_argument,		0,	'l'},
	{"compress",	no_argument,			0,	'c'},
	{0, 0, 0, 0}
};

//buffer variables
unsigned char buf[256];		//read input into 256 size buf
unsigned char cbuf[256];	//compressed buffer

int size = 0;
const char crlr[2] = "\r\n";

//declare pollfd struct
struct pollfd poll_in[2]; // {stdin, socket}

//set up z_stream for compression
z_stream stdin_stream;
z_stream server_stream;

//argument parsing variables
int ind = 0;
int ch;

//helper functions
void reset_term();
void set_up_terminal();
void set_up_socket();
void z_stream_setup();

int main(int argc, char *argv []){
	//argument parsing
	while ( (ch = getopt_long(argc, argv, "p:l:c", arr_option, &ind) ) != -1){
		switch (ch){
			case 'p':
				p_arg = atoi(optarg);
				p_flag = 1;
				break;
			case 'l':
				l_arg = optarg;
				l_flag = 1;
				break;
			case 'c':
				c_flag = 1;
				break;
			default:
				fprintf(stderr, "incorrect usage: lab1b-client [--port=port-number] [--log=filename] [--compress]\n");
				exit(1);
		}
	}

	//check to make sure port number was given
	if(p_arg == -1){
		fprintf(stderr, "error port-number not given\n");
		exit(1);
	}

	//set up log file
	if(l_flag && l_arg != NULL){
		if((logfd = open(l_arg, O_WRONLY | O_APPEND | O_CREAT, 0666)) < 0){
			fprintf(stderr, "error cannot open/create log file\n");
			exit(1);
		}
	}

	//set limit on file size of log
	struct rlimit l_limit = {10000, 10000};
	if(setrlimit(RLIMIT_FSIZE, &l_limit)){
		fprintf(stderr, "error cannot set rlimit\n");
		exit(1);
	}

	//set up the terminal
	set_up_terminal();
	//when program ends reset terminal
	atexit(reset_term);
	//set up the socket
	set_up_socket();
	//setup compression
	z_stream_setup();

	//set up file descriptors
	poll_in[0].fd = STDIN_FILENO;
	poll_in[1].fd = sockfd;

	//set up events
	poll_in[0].events = POLLIN;
	poll_in[1].events = POLLIN | POLLHUP | POLLERR;

	//begin poll
	int poll_val = 0;
	int size_compressed_write = 0;
	while (1){
		//poll the input streams
		if((poll_val = poll(poll_in, 2, -1)) < 0){
			fprintf(stderr, "error with poll: %s\n", strerror(errno));
			exit(1);
		}
		//no input detected with poll
		else if(poll_val == 0){
			continue;
		}

		//holdup or error from socket
		if(poll_in[1].revents & (POLLHUP | POLLERR)){
			fprintf(stderr, "connection to server has stopped: POLLHUP or POLLERR on socket\n");
			shutdown(sockfd, SHUT_RDWR);
			break;
		}

		//read from stdin
		if(poll_in[0].revents & POLLIN){
			if((size = read(STDIN_FILENO, buf, 256)) < 0){
				fprintf(stderr, "error cannot read from stdin: %s\n", strerror(errno));
				shutdown(sockfd, SHUT_RDWR);
				break;
			}

			if(size == 0){
				shutdown(sockfd, SHUT_RDWR);
				break;
			}

			for(int i = 0; i < size; i++){
				//newline or carriage return
				if(buf[i] == '\n' || buf[i] == '\r'){
					write(STDOUT_FILENO, crlr, 2);
				}

				//^C
				else if(buf[i] == 0x03){
					write(STDOUT_FILENO, "^C", 2);
					write(STDOUT_FILENO, crlr, 2);
				}

				//^D
				else if(buf[i] == 0x04){
					write(STDOUT_FILENO, "^D", 2);
					write(STDOUT_FILENO, crlr, 2);
				}

				//write 1 char at a time normally
				else {
					write(STDOUT_FILENO, buf + i, 1);
				}
			}

			if(c_flag && size > 0){
				stdin_stream.avail_in	= size;
				stdin_stream.next_in	= buf;
				while(stdin_stream.avail_in > 0){
					stdin_stream.avail_out	= 256;
					stdin_stream.next_out	= cbuf;
					
					//deflate stdin
					deflate(&stdin_stream, Z_SYNC_FLUSH);

					//get length of write
					int size_compressed_write = 256-stdin_stream.avail_out;

					//write to compressed buffer
					if(write(sockfd, cbuf, size_compressed_write) == EPIPE){
						shutdown(sockfd, SHUT_RDWR);
					}

					//log to the log file
					if(l_flag && size > 0 && *buf != 0x04 && *buf != 0x03){
						//begining of line
						dprintf(logfd, "SENT %d bytes: ", size_compressed_write);
						//write the regular buffer
						write(logfd, cbuf, size_compressed_write);
						//add newline
						write(logfd, &crlr[1], 1);
					}
				}
			}
			else{
				//write normally
				if(write(sockfd, buf, size) == EPIPE){
					shutdown(sockfd, SHUT_RDWR);
					break;
				}
				//write to log
				if(l_flag && size > 0 && *buf != 0x04 && *buf != 0x03){
					//begining of line
					dprintf(logfd, "SENT %d bytes: ", size);
					//write the regular buffer
					write(logfd, buf, size);
					//add newline
					write(logfd, &crlr[1], 1);
				}
			}
		}

		//read from socket
		if(poll_in[1].revents & POLLIN){
			//read from socket
			if((size = read(sockfd, buf, 256)) < 0){
				fprintf(stderr, "error cannot read from socket: %s\n", strerror(errno));
				shutdown(sockfd, SHUT_RDWR);
				exit(1);
			}

			//write to the log file
			if(l_flag && size > 0){
				//begining of line
				dprintf(logfd, "RECEIVED %d bytes: ", size);
				//write the buffer
				write(logfd, buf, size);
				//add newline
				write(logfd, &crlr[1], 1);
			}

			if(size == 0){
				shutdown(sockfd, SHUT_RDWR);
				break;
			}

			//check if compressed
			if(c_flag){
				//set up the variables related to compression
				server_stream.avail_in 	= size;
				server_stream.next_in	= buf;

				while(server_stream.avail_in > 0){
					server_stream.next_out	= cbuf;
					server_stream.avail_out	= 256;

					//inflate server stream
					inflate(&server_stream, Z_SYNC_FLUSH);
					size_compressed_write = 256-server_stream.avail_out;

					//loop through and check for newlines and ^D
					for(int i = 0; i < size_compressed_write; i++){
						//newline or carriage return
						if(cbuf[i] == '\n' || cbuf[i] == '\r'){
							write(STDOUT_FILENO, crlr, 2);
						}
						//^D
						else if(cbuf[i] == 0x04){
							write(STDOUT_FILENO, "^D", 2);
							write(STDOUT_FILENO, crlr, 2);
							break;
						}
						//regular writing
						else{
							write(STDOUT_FILENO, cbuf + i, 1);
						}
					}
				}
			}
			//if not compressed
			else{
				for(int i = 0; i < size; i++){
					//newline or carriage return
					if(buf[i] == '\n' || buf[i] == '\r'){
						write(STDOUT_FILENO, crlr, 2);
					}
					//^D
					else if(buf[i] == 0x04){
						write(STDOUT_FILENO, "^D", 2);
						write(STDOUT_FILENO, crlr, 2);
						break;
					}
					//regular writing
					else{
						write(STDOUT_FILENO, buf + i, 1);
					}
				}
			}
		}
	}

	//if compression was on deflate and inflate
	if(c_flag){
		deflateEnd(&stdin_stream);
		inflateEnd(&server_stream);
	}
	exit(0);
}

//definitions of helper functions
void reset_term(){
	if(tcsetattr(STDIN_FILENO, TCSANOW, &stdin_attr_orig) < 0){
		fprintf(stderr,"cannot restore original attributes for stdin: %s\n", strerror(errno));
		exit(1);
	}
}

void set_up_terminal(){
	//get attributes for stdin
	if(tcgetattr(STDIN_FILENO, &stdin_attr) < 0){
		fprintf(stderr,"cannot get attributes for stdin: %s\n", strerror(errno));
		exit(1);
	}

	//make a copy
	stdin_attr_orig = stdin_attr;

	//when program ends reset terminal
	atexit(reset_term);

	//change attributes for stdin
	stdin_attr.c_iflag = ISTRIP;	/* only lower 7 bits*/
	stdin_attr.c_oflag = 0;			/* no processing	*/
	stdin_attr.c_lflag = 0;			/* no processing	*/

	//set new attributes
	if(tcsetattr(STDIN_FILENO, TCSANOW, &stdin_attr) < 0){
		fprintf(stderr,"cannot set new attributes for stdin: %s\n", strerror(errno));
		exit(1);
	}
}

void set_up_socket(){
	//make the socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "error socket could not be opened on client side\n");
		exit(1);
	}

	//set up server address
	bzero((char *) &address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(p_arg);
	address.sin_addr.s_addr = INADDR_ANY;

	// //copy over the s_addr from the server entity
	// bcopy((char*) server_ent->h_addr,(char*) &address.sin_addr.s_addr, server_ent->h_length);

	//connect to server
	if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
		fprintf(stderr, "error with connect on client side\n");
		exit(1);
	}
}

void z_stream_setup(){
	//set up compression vars
	//set up for stdin
	stdin_stream.zalloc 	= Z_NULL;
	stdin_stream.zfree 		= Z_NULL;
	stdin_stream.opaque 	= Z_NULL;

	//set up for server stream
	server_stream.zalloc 	= Z_NULL;
	server_stream.zfree 	= Z_NULL;
	server_stream.opaque 	= Z_NULL;
	server_stream.avail_in 	= 0;
	server_stream.next_in	= Z_NULL;

	//initialize deflate stdin zstream with default compression
	if(deflateInit(&stdin_stream, Z_DEFAULT_COMPRESSION) != Z_OK){
		fprintf(stderr, "error cannot deflate of stdin zstream\n");
		exit(1);
	}
	if(!c_flag){
		deflateEnd(&stdin_stream);
	}

	//initialize inflate server zstream
	if(inflateInit(&server_stream) != Z_OK){
		fprintf(stderr, "error cannot inflate of server zstream\n");
		exit(1);
	}
	if(!c_flag){
		inflateEnd(&server_stream);
	}
}