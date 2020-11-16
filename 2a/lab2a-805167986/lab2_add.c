// William Randall
// wrandall1000@gmail.com
// 805167986

#include <pthread.h> //pthread
#include <stdio.h> //fprintf
#include <stdlib.h> //exit
#include <string.h> //strerror //atoi

#include <getopt.h> //getopt
#include <time.h>

//GLOBAL VARS

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
volatile int spin_lock = 0;
volatile int compare_swap_lock = 0;

//flag variables
int t_flag = 0;
int i_flag = 0;
int s_flag = 0;
int opt_yield = 0;

//argument variables
int t_arg 	= 1;
int i_arg	= 1;
char s_arg	= 0;

//GLOBAL STRUCT
static struct option arr_option[] =
{
	{"threads",		required_argument,		0,	't'},
	{"iterations",	required_argument,		0,	'i'},
	{"sync",		required_argument,		0,	's'},
	{"yield",		no_argument,			0,	'y'},
	{0, 0, 0, 0}
};

struct addParameters {
	int iterations;
	char thread_type;
	long long * counter_pointer;
};
typedef struct addParameters addParameters_t;


//HELPER FUNCTIONS
//general add function to call the other types of adds based on --sync
void * general_add(void * parameters);
//add but it is not protected
void add(long long *pointer, long long value);
//add protected with mutex
void add_m(long long *pointer, long long value);
//add protected with sync
void add_s(long long *pointer, long long value);
//add protected with compare and swap
void add_c(long long *pointer, long long value);

int main(int argc, char ** argv)
{	
	int ind = 0;
	int ch = 0;
	//argument parsing
	while ( (ch = getopt_long(argc, argv, "t:i:s:y", arr_option, &ind) ) != -1){
		switch (ch){
			case 't':
				t_flag = 1;
				t_arg = atoi(optarg);
				break;
			case 'i':
				i_flag = 1;
				i_arg = atoi(optarg);
				break;
			case 's':
				s_flag = 1;
				s_arg = *optarg;
				break;
			case 'y':
				opt_yield = 1;
				break;
			default:
				fprintf(stderr, "incorrect usage: lab2_add [--threads=#] [--iterations=#] [--sync=s|m|c] [--yield]\n");
				exit(1);
		}
	}

	//set up variables
	long long counter = 0;

	//make timespec structures
	struct timespec start_time;
	struct timespec end_time;

	//set up expected tag fields
	char name[15];
	if(opt_yield){
		strcpy(name, "add-yield-");
	}
	else{
		strcpy(name, "add-");
	}
	if(s_flag){
		if(s_arg == 'm'){
			strcat(name, "m\0");
		}
		else if(s_arg == 's'){
			strcat(name, "s\0");
		}
		else if(s_arg == 'c'){
			strcat(name, "c\0");
		}
		else{
			fprintf(stderr, "incorrect argument to --sync: lab2_add [--sync=s|m|c] ...\n");
			exit(1);
		}
	}
	else{
		strcat(name, "none\0");
	}

	//make a struct with the correct parameters
	addParameters_t parameters;
	parameters.iterations = i_arg;
	parameters.thread_type = s_arg;
	parameters.counter_pointer = &counter;

	//start time
	clock_gettime(CLOCK_REALTIME, &start_time);

	//make a pthread array of variable size
	pthread_t * pthread_array = malloc(sizeof(pthread_t) * t_arg);
	
	//create each thread
	for(int i = 0; i < t_arg; i++){
		pthread_create(&pthread_array[i], NULL, general_add, &parameters);
	}

	//join the threads back together
	for(int i = 0; i < t_arg; i++){
		pthread_join(pthread_array[i], NULL);
	}

	//free the pthread array
	free(pthread_array);

	//end time
	clock_gettime(CLOCK_REALTIME, &end_time);

	//time in seconds
	long seconds = end_time.tv_sec - start_time.tv_sec;

	//time in nanoseconds
	long nano_seconds = end_time.tv_nsec - start_time.tv_nsec;

	//handle underflow for nanoseconds
	if(start_time.tv_nsec > end_time.tv_nsec){
		seconds -= 1;
		nano_seconds += 999999999 + 1;
	}

	//output in csv format
	// the name of the test (add-none for the most basic usage)
	printf("%s,",name);

	// the number of threads (from --threads=)
	printf("%d,",t_arg);

	// the number of iterations (from --iterations=)
	printf("%d,",i_arg);

	// the total number of operations performed (threads x iterations x 2, the "x 2" factor because you add 1 first and then add -1)
	int num_operations = t_arg * i_arg * 2;
	printf("%d,",num_operations);

	// the total run time (in nanoseconds)
	long total_time = seconds * 1000000000 + nano_seconds;
	printf("%ld,",total_time);

	// the average time per operation (in nanoseconds).
	long average_time = total_time / num_operations;
	printf("%ld,",average_time);

	// the total at the end of the run (0 if there were no conflicting updates)
	printf("%lld\n",counter);

	return 0;
}

//required function implementations
void * general_add(void * parameters){
	addParameters_t * ptr_add_parameters = (addParameters_t * ) parameters;
	
	//create flags
	int m = 0;
	int s = 0;
	int c = 0;

	if(s_flag){
		switch(ptr_add_parameters->thread_type){
			case 'm':
				m = 1;
				break;
			case 's':
				s = 1;
				break;
			case 'c':
				c = 1;
				break;
			default:
				fprintf(stderr, "ERROR INVALID --SYNC PARAMETER\n");
				exit(1);
		}
	}

	int add_or_sub = 1;
	for(int j = 0; j < 2; j++){
		for(int i = 0; i < ptr_add_parameters->iterations; i++){
			if(!s_flag && !m && !s && !c){
				add(ptr_add_parameters->counter_pointer, add_or_sub);
			}
			//perform mutex operation
			else if(m && !s && !c){
				add_m(ptr_add_parameters->counter_pointer, add_or_sub);
			}
			//perform spin operation
			else if(!m && s && !c){
				add_s(ptr_add_parameters->counter_pointer, add_or_sub);
			}
			//perform compare and swap operation
			else if(!m && !s && c){
				add_c(ptr_add_parameters->counter_pointer, add_or_sub);
			}
			//error
			else{
				fprintf(stderr, "ERROR INVALID SYNTAX\n");
				exit(1);
			}
		}
		add_or_sub = -1;
	}
	
	return NULL;
}

void add(long long *pointer, long long value) {
	long long sum = *pointer + value;
	if (opt_yield)
		sched_yield();
	*pointer = sum;
}

void add_m(long long *pointer, long long value) {
	//lock mutex
	if(pthread_mutex_lock(&mutex_lock)){
		fprintf(stderr, "error locking mutex\n");
		exit(2);
	}
	//perform add
	long long sum = *pointer + value;
	if(opt_yield)
		sched_yield();
	*pointer = sum;

	//unlock mutex
	if(pthread_mutex_unlock(&mutex_lock)){
		fprintf(stderr, "error unlocking mutex\n");
		exit(2);
	}
}

void add_s(long long *pointer, long long value) {
	//set up the spin lock
	while(__sync_lock_test_and_set(&spin_lock, 1)){
		while(spin_lock);
	}

	//perform add
	long long sum = *pointer + value;
	if (opt_yield)
		sched_yield();
	*pointer = sum;

	//release the spin lock
	__sync_lock_release(&spin_lock);
}

void add_c(long long *pointer, long long value) {
	//get old value
	long long old_value = *pointer;
	long long sum = 0;

	// perform add with no protection
	sum = old_value + value;
	if (opt_yield)
			sched_yield();

	while(__sync_val_compare_and_swap(pointer, old_value, sum) != old_value){
		//if the old value is not synced over update and perform add again
		old_value = *pointer;
		sum = old_value + value;
		if (opt_yield)
			sched_yield();
	}
}