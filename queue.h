/* Queue for displaying the operations */
#include "header.h"

typedef struct operation {
	char op;
	int x, y;
} operation;

typedef struct node {
	operation o;
	struct node *next, *prev;
} node;

typedef struct queue {
	node *head, *tail;
} queue;

/* List of all used functions for extended queue */
void qinit (queue* );
void enq (queue* , operation );
operation deq (queue* );
int qfull (queue );
int qempty (queue );
void qtraverse (queue );
void qprint (queue* , lineInfo* , lineInfo* );
void qytraverse (queue );
void qyprint (queue* , lineInfo* , lineInfo* );
void qctraverse (queue );
void qcprint (queue* , lineInfo* , lineInfo* , char* , char* );
void printcdiff (lineInfo* , lineInfo* , char* , char* );
void remprint (lineInfo* , lineInfo* );
char qpeek (queue* );
int qlength (queue* );
int tabs (char* );
