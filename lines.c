/* Utility functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hpatch.h"

static int len;

//Initializes the pointer variable
void init (contents* str) {
	str = NULL;
}

/* Finds the number of lines in the file */
int length (contents* str) {
	lineInfo *ptr = *str;
	int len = 0;
	while(ptr) {
		ptr = ptr->next;
		len++;
	}
	return len;
}

/* 
 * Use this function to read all the lines from a file,
 * and store them into continuous lineInfo pointers for comparison.
 */
contents* lines (char* file) {
	FILE *fp;
	fp = fopen (file, "r");
	if (fp == NULL) {
		exit(0);
	}
	size_t size = 256;
	int c = 1;
	char *line = NULL;
	contents* str = (contents*) malloc (sizeof(contents));
	while (1) {
		// Till lines exist, malloc string pointers and store the lines
		line = (char*) malloc (sizeof(char) * size);
		if(getline (&line, &size, fp) == -1)
			break;
		insertString (str, line, c, 1);
		c++;
		free(line);
	}
	fclose (fp);
	len = length(str);
	return str;
}

/* Inserts a string into the sequence of lines */
void insertString (contents* str, char* line, int pos, int flag) {
	lineInfo *new, *ptr = *str;
	if (pos > max(str) + 1 || pos < 1)
		return;
	if (!ptr) {
		if (pos != 1)
			return;
		new = (lineInfo*) malloc (sizeof(lineInfo));
		new->lineText = (char*) malloc (sizeof(char) * 256);
		strcpy (new->lineText, line);
		if (*((new->lineText) + strlen(line) - 1) == '\n')
			*((new->lineText) + strlen(line) - 1) = '\0';
		if(flag)
			new->lineNumber = pos;
		else
			new->lineNumber = len + pos;
		*str = new;
		new->next = NULL;
		return;
	}
	if(pos == 1) {
		new = (lineInfo*) malloc (sizeof(lineInfo));
		new->lineText = (char*) malloc (sizeof(char) * 256);
		strcpy (new->lineText, line);
		if (*((new->lineText) + strlen(line) - 1) == '\n')
			*((new->lineText) + strlen(line) - 1) = '\0';
		if(flag)
			new->lineNumber = pos;
		else
			new->lineNumber = len + pos;
		new->next = *str;
		*str = new;
		return;
	}
	while (pos - 1 != ptr->lineNumber)
		ptr = ptr->next;
	new = (lineInfo*) malloc (sizeof(lineInfo));
	new->lineText = (char*) malloc (sizeof(char) * 256);
	strcpy (new->lineText, line);
	if (*((new->lineText) + strlen(line) - 1) == '\n')
		*((new->lineText) + strlen(line) - 1) = '\0';
	if(flag)
		new->lineNumber = pos;
	else
		new->lineNumber = len + pos;
	new->next = ptr->next;
	ptr->next = new;
}

/* Deletes a particular string from the sequence of lines */
void deleteString (contents* str, int num) {
	lineInfo *ptr = *str, *temp = NULL;
	while(ptr && ptr->lineNumber != num) {
		temp = ptr;
		ptr = ptr->next;
	}
	if(!ptr)
		return;
	if(ptr == *str) {
		*str = ptr->next;
		free(ptr);
		return;
	}
	if(ptr->next == NULL) {
		temp->next = NULL;
		free(ptr);
		return;
	}
	temp->next = ptr->next;
	free(ptr);
}

/* Prints the contents to the specified file */
void traverse (contents str, char* file) {
	FILE *fp;
	fp = fopen (file, "w+");
	lineInfo* ptr = str;
	while(ptr) {
		fprintf(fp, "%s\n", ptr->lineText);
		ptr = ptr->next;
	}
	fclose (fp);
}

/* Gets the string corresponding to the specified line number */
char* string (contents* str, int num) {
	lineInfo* ptr = *str;
	char* line;
	while(ptr) {
		if(ptr->lineNumber == num)
			line = ptr->lineText;
		ptr = ptr->next;
	}
	return line;
}

/* Finds the max line number */
int max (contents* str) {
	lineInfo* ptr = *str;
	int m = 0;
	while(ptr) {
		if(ptr->lineNumber > m)
			m = ptr->lineNumber;
		ptr = ptr->next;
	}
	return m;
}
