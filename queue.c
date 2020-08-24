/* Queue Utility Definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

//Initializes the queue
void qinit (queue *q) {
	q->head = NULL;
	q->tail = NULL;
}

//Enqueues operations
void enq (queue *q, operation o) {
	node* new = (node*) malloc (sizeof(node));
	new->o = o;
	new->next = NULL;
	if (q->head == NULL)
		q->head = new;
	else
		q->tail->next = new;
	new->prev = q->tail;
	q->tail = new;
}

//Returns operations one at a time for processing
operation deq (queue *q) {
	node* ptr = q->head;
	operation o = ptr->o;
	if (q->head == q->tail) {
		q->head = q->tail = NULL;
	}
	else {
		q->head = ptr->next;
		q->head->prev = NULL;
	}
	free(ptr);
	return o;
}

//Checks for full queue
int qfull (queue q) {
	node* new = (node*) malloc (sizeof(node));
	if (new == NULL)
		return 1;
	free(new);
	return 0;
}

//Checks for empty queue
int qempty (queue q) {
	return (q.head == NULL);
}

//Returns the character stored in the following operation
char qpeek (queue* q) {
	if(q->head->next == NULL)
		return 'q';
	else
		return q->head->next->o.op;
}

//Returns the length of the queue
int qlength (queue* q) {
	node* ptr = q->head;
	int l = 0;
	while(ptr) {
		l++;
		ptr = ptr->next;
	}
	return l;
}
