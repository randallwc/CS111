// William Randall
// wrandall1000@gmail.com
// 805167986

#include "SortedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element){
	//get a current node
	SortedListElement_t * current = list->next;

	//loop until the current node's key is equal to NULL
	while(current->key != NULL){
		//check if the current's key is less than the key you are trying to find
		if(strcmp(current->key, element->key) < 0){
			break;
		}

		//sort in ascending order based on key
		current = current->next;
	}

	//check for opt_yield
	if(opt_yield & INSERT_YIELD)
		sched_yield();

	//point element's previous to current's previous
	element->prev = current->prev;

	//point the current's previous at element
	current->prev = element;

	//check for opt_yield
	if(opt_yield & INSERT_YIELD)
		sched_yield();

	//point element's next at the current node in current
	element->next = current;

	//point the current node's next to element
	element->prev->next = element;
}

int SortedList_delete(SortedListElement_t *element){
	//check edge conditions
	if(element->prev->next != element){
		return 1;
	}
	if(element->next->prev != element){
		return 1;
	}

	//point the previous's next to element's next
	element->prev->next = element->next;

	//check for opt_yield
	if(opt_yield & DELETE_YIELD)
		sched_yield();

	//point element's next's previous to element's previous
	element->next->prev = element->prev;

	return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key){
	//get a current node
	SortedListElement_t * current = list->next;

	//get first non head node
	SortedListElement_t * first = current;

	//while current is not null
	while(current != NULL){
		//if you found the key break
		if(current->key == key){
			break;
		}

		//else continue
		current = current->next;
		
		//if you looped all the way around stop and return NULL
		if(current == first){
			return NULL;
		}
	}

	//check for opt_yield
	if(opt_yield & LOOKUP_YIELD)
		sched_yield();

	return current;
}

int SortedList_length(SortedList_t *list){
	int count = 0;

	//get a current node
	SortedListElement_t * current = list->next;

	//while current->key is not NULL	
	while(current->key != NULL){
		//check if both next's prev and prev's next point to current
		if(current->prev->next != current){
			return -1;
		}
		if(current->next->prev != current){
			return -1;
		}

		count++;
		current = current->next;
	}

	//check for opt_yield
	if(opt_yield & LOOKUP_YIELD)
		sched_yield();

	return count;
}