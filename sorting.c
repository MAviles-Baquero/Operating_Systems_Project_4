/*
	Maria Elena Aviles-Baquero
	Date: 11/05/2020
	CPSC 346: Operating Systems
	Project 4: Multithreaded Sorting Application
	
	To Compiler: > gcc -o sorting sorting.c -pthread
			   > ./sorting
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define NUMBER_OF_THREADS 3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct
{
	int from_index;
	int to_index;
} parameters;

int main (int argc, const char * argv[]) 
{
	int i, j;
	pthread_t workers[NUMBER_OF_THREADS];
	
	/* establish the first sorting thread */
	parameters *data = (parameters*)malloc(sizeof(parameters));
	
	data->from_index = 0;
	data->to_index = (SIZE / 2) - 1;
	
	/* Creates first thread */
	pthread_create(&workers[0], 0, sorter, data);

	/* establish the second sorting thread */
	data = (parameters*)malloc(sizeof(parameters));
	
	data->from_index = SIZE / 2;
	data->to_index = SIZE - 1;
	
	/* Displays unsorted array */
	printf("The unsorted array is: ");
	for(j = 0; j < SIZE; j++) {
		printf("%d ", list[j]);
	}
	printf("\n");
	
	/* Creates second thread */
	pthread_create(&workers[1], 0, sorter, data);
	
	/* now wait for the 2 sorting threads to finish */
	for(i = 0; i < NUMBER_OF_THREADS - 1; i++) {
		pthread_join(workers[i], NULL);
	}

	/* establish the merge thread */
	data = (parameters*)malloc(sizeof(parameters));
	data->from_index = 0;
	data->to_index = SIZE / 2;
	
	/* Create merging thread */
	pthread_create(&workers[2], 0, merger, data);
	
	/* wait for the merge thread to finish */
	pthread_join(workers[2], NULL);
	
	return 0;
}


/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
	parameters* p = (parameters*)params;
	
	int i, j, k, l, m;
	int start = p->from_index;
	int end = p->to_index + 1;

	for(j = start; j < end; j++) {
		for(k = start; k < (end-j-1); k++) {
			if(list[k] > list[k+1]) {
				l = list[k];
				list[k] = list[k+1];
				list[k+1] = l;
			}
		}
	}
	
	for(m = start; m < end; m++)
		result[m] = list[m];
	
	pthread_exit(0);
}


/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{
	parameters* p = (parameters *)params;
	
	int i, j, k, l;
	int start = p->from_index;
	int end = p->to_index + 1;
	
	for(i = start; i < end; i++) {
		for(j = start; j < end-i; j++) {
			if(result[j] > result[j+1]) {
				k = result[j];
				result[j] = result[j+1];
				result[j+1] = k;
			}
		}
	}
	
	printf("The final array is: ");
	for(l = start; l < end; l++) 
		printf("%d ", result[l]);
	printf("\n");
	
	pthread_exit(0);
}


