/* Linked List for storing the operations */
#include "queue.h"

typedef struct list {
	node *head, *tail;
} list;

//Function prototypes
void init (list* );
void insert (list* , int , int , char );
void traverse (list* , lineInfo* , lineInfo* );
void destroy (list* );
int isEmpty (list );
