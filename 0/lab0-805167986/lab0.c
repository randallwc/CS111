// NAME: William Randall
// EMAIL: wrandall1000@gmail.com
// ID: 805167986

#include <fcntl.h> //open & creat
#include <unistd.h> //close
#include <stdlib.h> //exit
#include <getopt.h> //getopt
#include <signal.h> //signal
#include <stdio.h> //fprintf
#include <errno.h> //errno
#include <string.h> //strerror

//exit codes//
int unrec_arg = 1;
int input_err = 2;
int out_err = 3;
int sig_err = 4;

void segfault () {
	char* bad_ptr = NULL;
	*bad_ptr = 'a'; //dereferences null
}

void segfault_handler (int signum) {
	fprintf(stderr,"segmentation fault signum:\"%d\"\n", signum);
	exit(sig_err);
}

int main (int argc, char *argv []){
	//flags//
	int i_flag = 0;
	int o_flag = 0;
	int s_flag = 0;
	int c_flag = 0;

	static struct option arr_option[] =
	{
		{"input",		required_argument,	0, 'i'},
		{"output",		required_argument,	0, 'o'},
		{"segfault",	no_argument,		0, 's'},
		{"catch",		no_argument,		0, 'c'},
		{0, 0, 0, 0}
	};

	int ind = 0;
	int ch;
	int in_file_ind = 0;
	int out_file_ind = 0;

	while ( (ch = getopt_long(argc, argv, "i:o:sc", arr_option, &ind) ) != -1){
		switch (ch){
			case 'i':
					i_flag = 1;
					in_file_ind = open(optarg, O_RDONLY);
					if (in_file_ind < 0){
						//open failed
						fprintf(stderr,"error on --input on file: %s with error message \"%s\"\n",
							optarg,
							strerror(errno));
						exit(input_err);
					}
				break;
			case 'o':
				{
					o_flag = 1;
					out_file_ind = open(optarg, O_WRONLY | O_CREAT , 0666);//b110110110
					if (out_file_ind < 0){
						//output failed
						fprintf(stderr,"error on --output on file: %s with error message \"%s\"\n",
							optarg,
							strerror(errno));
						exit(out_err);
					}
				}
				break;
			case 's':
				s_flag = 1;
				break;
			case 'c':
				c_flag = 1;
				break;
			default:
				fprintf(stderr, "incorrect usage: lab0 [--input=filename] [--output=filename] [--segfault] [--catch]\n");
				exit(unrec_arg);
		}
	}

	if(i_flag){
		close(STDIN_FILENO);
		dup(in_file_ind);
		close(in_file_ind);
	}
	if(o_flag){
		close(STDOUT_FILENO);
		dup(out_file_ind);
		close(out_file_ind);
	}
	if(c_flag){
		signal(SIGSEGV, segfault_handler);
	}
	if(s_flag){
		segfault();
	}

	char buf;
	while ( read(STDIN_FILENO, &buf, 1) > 0 ){ // copying over the input to output file
		write(STDOUT_FILENO, &buf, 1);
	}
	
	return(0);
}