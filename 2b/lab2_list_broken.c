// William Randall
// wrandall1000@gmail.com
// 805167986

#include "SortedList.h" // SortedListElement

#include <pthread.h> //pthread
#include <stdio.h> //fprintf
#include <stdlib.h> //exit // rand
#include <string.h> //strerror //atoi
#include <signal.h> //signal
#include <getopt.h> //getopt
#include <time.h> //timespec //clock_gettime


//GLOBAL VARS
// #define MAX_ITERS 100000
// #define MAX_LISTS 100
#define OVERFLOW 1000000000
// pthread_mutex_t mutex_lock[MAX_LISTS]; // array of mutex locks
pthread_mutex_t * mutex_lock; // array of mutex locks
// volatile int spin_lock[MAX_LISTS] = {0}; // array of spin locks
volatile int * spin_lock; // array of spin locks

//flag variables
int t_flag = 0;
int i_flag = 0;
int s_flag = 0;
int l_flag = 0;
int opt_yield = 0;

//argument variables
int t_arg 	= 1;
int i_arg	= 1;
char s_arg	= 0;
int l_arg 	= 1;
char y_arg[4] = {0};

//GLOBAL STRUCT
static struct option arr_option[] =
{
	{"threads",		required_argument,		0,	't'},
	{"iterations",	required_argument,		0,	'i'},
	{"sync",		required_argument,		0,	's'},
	{"yield",		required_argument,		0,	'y'},
	{"lists",		required_argument,		0,	'l'},
	{0, 0, 0, 0}
};

struct listParameters {
	int iterations; // number of iterations
	char thread_type; // m or s
	int list_amt; // which list it is in
	SortedList_t* head_ptr; // pointer to the head node
	SortedListElement_t* first_element_ptr; // pointer to the first element
};
typedef struct listParameters listParameters_t;

// array of alphabet
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

// total time waiting for lock
long time_locked = 0;

//HELPER FUNCTIONS
//handler function for segfault
void segfault_handler(int error);
//make a random string for 5 chars
char * random_string();
//general thread function to call other thread functions
void * thread(void * parameters);
//perform lock on thread sync
void lock(int i, char ch);
//perform unlock on thread sync
void unlock(int i, char ch);
//choose which list to put each element in
int list_hash(int amt_lists, const char * str);

int main(int argc, char ** argv)
{	
	//set up segfault handler
	signal(SIGSEGV, segfault_handler);

	int ind = 0;
	int ch = 0;
	//argument parsing
	while ( (ch = getopt_long(argc, argv, "t:i:s:y:", arr_option, &ind) ) != -1){
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
				strcpy(y_arg, optarg);
				break;
			case 'l':
				l_flag = 1;
				l_arg = atoi(optarg);
				break;
			default:
				fprintf(stderr, "incorrect usage: lab2_list [--threads=#] [--iterations=#] [--sync=s|m] [--yield=[idl]]\n");
				exit(1);
		}
	}

	//malloc the mutex locks and the spin locks
	if(s_arg == 'm'){
		mutex_lock = (pthread_mutex_t *)malloc(l_arg * sizeof(pthread_mutex_t));
	}
	else if(s_arg == 's'){
		spin_lock = (volatile int *)calloc(l_arg, sizeof(volatile int));
	}

	// //limit how many iterations you can do
	// if(i_arg * t_arg > MAX_ITERS){
	// 	fprintf(stderr, "number of iterations too high\n");
	// 	exit(1);
	// }

	// //limit how many lists can be used
	// if(l_arg > MAX_LISTS){
	// 	fprintf(stderr, "number of lists too high\n");
	// 	exit(1);
	// }

	//set up the opt_yield value
	char *ch_ptr = y_arg;
	while(*ch_ptr != 0){
		char ch = *ch_ptr;
		if(ch == 'i'){
			// yield in insert critical section
			opt_yield += INSERT_YIELD;
		}
		else if(ch == 'd'){
			// yield in delete critical section
			opt_yield += DELETE_YIELD;
		}
		else if(ch == 'l'){
			// yield in lookup/length critical section
			opt_yield += LOOKUP_YIELD;
		}
		else{
			fprintf(stderr, "incorrect usage of --yield=[idl] argument\n");
			exit(1);
		}
		ch_ptr++;
	}

	//set up expected tag fields
	char name[15];
	strcpy(name, "list-");

	//check if there are opt_yield arguments
	if(opt_yield){
		if(opt_yield & INSERT_YIELD)
			strcat(name, "i");

		if(opt_yield & DELETE_YIELD)
			strcat(name, "d");

		if(opt_yield & LOOKUP_YIELD)
			strcat(name, "l");
	}
	else{
		strcat(name, "none");
	}

	strcat(name, "-");

	//check for sync options
	if(s_flag){
		if(s_arg == 'm'){
			strcat(name, "m\0");
		}
		else if(s_arg == 's'){
			strcat(name, "s\0");
		}
		else{
			fprintf(stderr, "incorrect usage of [--sync=s|m] argument\n");
			exit(1);
		}
	}
	else{
		strcat(name, "none\0");
	}

	//initiate mutex locks
	for(int i = 0; i < l_arg; i++){
		pthread_mutex_init(&mutex_lock[i], NULL);
	}

	//malloc a doubly linked list the size of threads * iterations
	SortedListElement_t * doubly_linked_list = malloc(t_arg * i_arg * sizeof(SortedListElement_t));
	// SortedListElement_t doubly_linked_list[MAX_ITERS];
	// char temp_key[6];
	// char temp_key[6] = {0};
	// char * temp_key = (char *)malloc(t_arg * i_arg * sizeof(char));
	for(int i = 0; i < (t_arg * i_arg); i++){
		doubly_linked_list[i].next = NULL;
		doubly_linked_list[i].prev = NULL;
		doubly_linked_list[i].key = random_string();
	}
	// //free the key
	// free(temp_key);

	//make an empty list
	SortedList_t * sorted_list = malloc(l_arg * sizeof(SortedList_t));
	// SortedList_t sorted_list[MAX_LISTS];
	for(int i = 0; i < l_arg; i++){
		//head points to itself
		sorted_list[i].prev = &sorted_list[i];
		sorted_list[i].next = &sorted_list[i];
		//head's key is NULL
		sorted_list[i].key = NULL;
	}

	//make timespec structures
	struct timespec start_time;
	struct timespec end_time;

	//start time
	clock_gettime(CLOCK_REALTIME, &start_time);

	//make a pthread array of variable size
	pthread_t * pthread_array = malloc(t_arg * sizeof(pthread_t));

	//make a parameter struct for each thread
	listParameters_t * parameter_array = malloc(t_arg * sizeof(listParameters_t));

	// listParameters_t
		// int iterations;
		// char thread_type;
		// SortedList_t* head_ptr;
		// SortedListElement_t* first_element_ptr;

	//create each thread
	for(int i = 0; i < t_arg; i++){
		//set up the parameters for each thread
		parameter_array[i].iterations 		 = i_arg; // number of iterations
		parameter_array[i].thread_type 		 = s_arg; // m or c
		parameter_array[i].head_ptr 		 = sorted_list; //pointer to the sorted list
		int index = i*i_arg;
		parameter_array[i].first_element_ptr = &doubly_linked_list[index]; //pointer to each element of the list
		parameter_array[i].list_amt		 	 = l_arg; // number of lists

		//create the thread and run the thread function
		pthread_create(&pthread_array[i], NULL, thread, &parameter_array[i]);
	}

	//join the threads back together
	for(int i = 0; i < t_arg; i++){
		pthread_join(pthread_array[i], NULL);
	}

	//free the pthread array
	free(pthread_array);

	//end time
	clock_gettime(CLOCK_REALTIME, &end_time);

	//check if every list is non zero
	for(int i = 0; i < l_arg; i++){
		if(SortedList_length(&sorted_list[i]) != 0){
			fprintf(stderr, "List has non-zero ending length\n");
			exit(2);
		}
	}

	//time in seconds
	long seconds = end_time.tv_sec - start_time.tv_sec;

	//time in nano seconds
	long nano_seconds = end_time.tv_nsec - start_time.tv_nsec;

	//handle underflow for nanoseconds
	if(start_time.tv_nsec > end_time.tv_nsec){
		nano_seconds += OVERFLOW;
		seconds -= 1;
	}

	//free sorted list
	free(sorted_list);

	//free dll
	free(doubly_linked_list);

	//free parameter array
	free(parameter_array);

	//free mutex locks
	free(mutex_lock);

	//free spin locks
	free((void *)spin_lock);

	//output in csv format
	// the name of the test, which is of the form: list-yieldopts-syncopts: where
	// yieldopts = {none, i,d,l,id,il,dl,idl}
	// syncopts = {none,s,m}
	printf("%s,",name);
	// the number of threads (from --threads=)
	printf("%d,",t_arg);
	// the number of iterations (from --iterations=)
	printf("%d,",i_arg);
	// the number of lists
	printf("%d,",l_arg);
	// the total number of operations performed: threads x iterations x 3 (insert + lookup + delete)
	int operations = t_arg * i_arg * 3;
	printf("%d,",operations);
	// the total run time (in nanoseconds) for all threads
	long total_time = seconds * OVERFLOW + nano_seconds;
	printf("%ld,",total_time);
	// the average time per operation (in nanoseconds).
	long average_time = total_time / operations;
	printf("%ld,",average_time);
	// the average time each thread has to wait for a lock
	if(s_flag == 0)
		time_locked = 0;
	long average_lock_wait = time_locked / (3 * t_arg * i_arg);
	printf("%ld\n",average_lock_wait);

	return 0;
}

//function implementations
int list_hash(int amt_lists, const char * str){
	int hash = 5381;
	int c;

	while ((c = *str++)){
		/* hash * 33 + c */
		hash = ((hash << 5) + hash) + c;
	}

	return hash % amt_lists;
}

void segfault_handler(int error){
	fprintf(stderr, "List is corrupted segfault code: %i", error);
	exit(2);
}

char * random_string()
{
	char * output_string = (char * )malloc(sizeof(char) * 6);
	int length = 5;
	while (length > 0)
	{
		int ind = (double) rand() / RAND_MAX * (26);
		*output_string = alphabet[ind];
		output_string++;
		length--;
	}
	*output_string = '\0';
	fprintf(stderr, "%s\n", output_string);
	return output_string;
}

//INCORRECT
// void random_string(char* output_string){
//     len++;
//     int index;
//     int length = 5;
//     for(int i = 0; i < length; i++){
//         //get a random index for alphabet string
//         index = (double) (rand() / RAND_MAX) * 26;
//         *output_string = alphabet[index - 1];
//         output_string++;
//     }
//     //add an end of string char
//     *output_string = '\0';
// }

void lock(int i, char ch){
	switch(ch){
		case 'm':
			//lock mutex //if it returns 0 that is good
			if(pthread_mutex_lock(&mutex_lock[i]) != 0){
				fprintf(stderr, "error on mutex lock\n");
				exit(2);
			}
			break;
		case 's':
			//lock for spin lock
			while(__sync_lock_test_and_set(&spin_lock[i], 1)){
				while(spin_lock[i]);
			}
			break;
		//do nothing to unlock
	}
}

void unlock(int i, char ch){
	switch(ch){
		case 'm':
			//unlock mutex //if it returns 0 that is good
			if(pthread_mutex_unlock(&mutex_lock[i]) != 0){
				fprintf(stderr, "error on mutex unlock\n");
				exit(2);
			}
			break;
		case 's':
			//unlock for spin lock
			__sync_lock_release(&spin_lock[i]);
			break;
		//do nothing to unlock
	}
}

void * thread(void * parameters){
	listParameters_t * arguments = (listParameters_t * ) parameters;

	//set up hash value
	int hash_val = 0;

	//set up thread time variable
	long thread_time = 0;

	//make timespec structures
	struct timespec start_time;
	struct timespec end_time;

	//insert all elements
	for(int i = 0; i < arguments->iterations; i++){
		//get which list to lock
		hash_val = list_hash(arguments->list_amt, arguments->first_element_ptr[i].key);

		//start time
		clock_gettime(CLOCK_REALTIME, &start_time);

		//lock during critical section
		lock(hash_val,arguments->thread_type);

		//end time
		clock_gettime(CLOCK_REALTIME, &end_time);

		//time in seconds
		long seconds = end_time.tv_sec - start_time.tv_sec;

		//time in nano seconds
		long nano_seconds = end_time.tv_nsec - start_time.tv_nsec;

		//handle underflow for nanoseconds
		if(start_time.tv_nsec > end_time.tv_nsec){
			nano_seconds += OVERFLOW;
			seconds -= 1;
		}

		//add to thread time variable
		thread_time += seconds * OVERFLOW + nano_seconds;

		//insert all the elements into the empty list
		SortedList_insert(&arguments->head_ptr[hash_val], &arguments->first_element_ptr[i]);

		//unlock after critical section
		unlock(hash_val,arguments->thread_type);
	}

	for(int i = 0; i < arguments->list_amt; i++){
		//start time
		clock_gettime(CLOCK_REALTIME, &start_time);

		//lock during critical section
		lock(i ,arguments->thread_type);

		//end time
		clock_gettime(CLOCK_REALTIME, &end_time);

		//time in seconds
		long seconds = end_time.tv_sec - start_time.tv_sec;

		//time in nano seconds
		long nano_seconds = end_time.tv_nsec - start_time.tv_nsec;

		//handle underflow for nanoseconds
		if(start_time.tv_nsec > end_time.tv_nsec){
			nano_seconds += OVERFLOW;
			seconds -= 1;
		}

		//add to thread time variable
		thread_time += seconds * OVERFLOW + nano_seconds;

		//check the list
		if(SortedList_length(&arguments->head_ptr[i]) == -1){
			fprintf(stderr, "List length is wrong. List is incorrect.\n");
			exit(2);
		}

		//unlock after critical section
		unlock(i ,arguments->thread_type);
	}

	//look up and delete each element
	for(int i = 0; i < arguments->iterations; i++){
		//get which list to lock
		hash_val = list_hash(arguments->list_amt, arguments->first_element_ptr[i].key);

		//start time
		clock_gettime(CLOCK_REALTIME, &start_time);

		//lock during critical section
		lock(hash_val,arguments->thread_type);

		//end time
		clock_gettime(CLOCK_REALTIME, &end_time);

		//time in seconds
		long seconds = end_time.tv_sec - start_time.tv_sec;

		//time in nano seconds
		long nano_seconds = end_time.tv_nsec - start_time.tv_nsec;

		//handle underflow for nanoseconds
		if(start_time.tv_nsec > end_time.tv_nsec){
			nano_seconds += OVERFLOW;
			seconds -= 1;
		}

		//add to thread time variable
		thread_time += seconds * OVERFLOW + nano_seconds;

		//look up element
		SortedListElement_t * element = SortedList_lookup(&arguments->head_ptr[hash_val], arguments->first_element_ptr[i].key);

		//check if the element is NULL
		if(element == NULL){
			fprintf(stderr, "List is corrupted.  Error during lookup.  Null poiter access.\n");
			exit(2);
		}

		//delete the element
		if(SortedList_delete(element) == 1){
			fprintf(stderr, "List is corrupted.  Error during delete.\n");
			exit(2);
		}

		//unlock after critical section
		unlock(hash_val,arguments->thread_type);
	}

	//update global time
	__sync_add_and_fetch(&time_locked, thread_time);

	return NULL;
}