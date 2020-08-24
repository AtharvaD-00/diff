/* Linked List Definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

//Flag variables from main file
extern int yFlag, cFlag, tFlag;

void init (list* l) {
	l->head = NULL;
	l->tail = NULL;
}

//Inserts an operation into the list
void insert (list* l, int x, int y, char c) {
	node* new = (node*) malloc (sizeof(node));
	new->o.x = x;
	new->o.y = y;
	new->o.op = c;
	new->next = NULL;
	node* ptr = l->head;
	if(!ptr) {
		l->head = new;
		l->tail = new;
		new->prev = NULL;
		return;
	}
	l->tail->next = new;
	new->prev = l->tail;
	l->tail = new;
}

//Traverses the list and calls the queue functions as needed
void traverse (list* l, lineInfo* str1, lineInfo* str2) {
	queue q;
	qinit(&q);
	node *ptr1 = l->head;
	//For the c flag
	char *print1 = (char*) malloc (sizeof(char) * size(str1) + 6), *print2 = (char*) malloc (sizeof(char) * size(str2) + 6);
	memset (print1, 'n', sizeof(char) * size(str1) + 6);
	memset (print2, 'n', sizeof(char) * size(str2) + 6);
	if(tFlag) {
		tabsToSpaces(str1);
		tabsToSpaces(str2);
	}
	while(ptr1) {
		enq (&q, ptr1->o);
		if(!ptr1->next || (ptr1->next->o.x > ptr1->o.x && ptr1->next->o.y > ptr1->o.y)) {
			if (yFlag) {
				qytraverse (q);
				qyprint (&q, str1, str2);
			}
			else if (cFlag) {
				qctraverse (q);
				qcprint (&q, str1, str2, print1, print2);
			}
			else {
				qtraverse (q);
				qprint (&q, str1, str2);
			}
			qinit (&q);
		}
		ptr1 = ptr1->next;
	}
	if(yFlag)
		remprint (str1, str2);
	if(cFlag)
		printcdiff (str1, str2, print1, print2);
}

//Destroys the list
void destroy (list* l) {
	node *ptr1 = l->head, *ptr2;
	while(ptr1) {
		ptr2 = ptr1;
		ptr1 = ptr1->next;
		free(ptr2);
	}
}

//Checks for empty list
int isEmpty (list l) {
	return (l.head == NULL);
}
