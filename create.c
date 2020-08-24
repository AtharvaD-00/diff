/* Function to use the created diff and insert it into a list */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* Prints the diff formed by backtracking */
void createDiff (editMap ed, lineInfo* str1, lineInfo* str2) {
	editRoute* ptr = ed;
	//Use a list to store additions and deletions
	list l;
	init(&l);
	while(ptr) {
		if(ptr->mid.x > ptr->start.x)
			insert (&l, ptr->mid.x, ptr->mid.y, 'd');
		if(ptr->mid.y > ptr->start.y)
			insert (&l, ptr->mid.x, ptr->mid.y, 'a');
		ptr = ptr->next;
	}
	//Now, traverse the list and use its contents to print the diff
	traverse(&l, str1, str2);
}
