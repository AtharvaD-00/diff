/* Utility functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

/* Finds out the number of lines in the array of lineInfo instances */
int size (lineInfo* str) {
	int c = 0;
	while (strcmp (str[c].lineText, "\0"))
		c++;
	return c;
}

/* 
 * Use this function to read all the lines from a file,
 * and store them into continuous lineInfo pointers for comparison.
 */
lineInfo* lines (char* file) {
	FILE *fp;
	fp = fopen (file, "r");
	if (fp == NULL) {
		exit(0);
	}
	size_t size = 256, lines = 256;
	int c = 0;
	char *line = NULL;
	lineInfo* str = (lineInfo*) malloc (sizeof(lineInfo) * lines);
	while (fp) {
		// Till lines exist, malloc string pointers and store the lines
		line = (char*) malloc (sizeof(char) * size);
		int res = getline (&line, &size, fp);
		if(*line == '\n') {
			strcat(line, " ");
			goto copy;
		}
		if(res == -1)
			break;
		copy:
		(str + c)->lineText = (char*) malloc (sizeof(char) * 256);
		strcpy ((str + c)->lineText, line);
		*((str + c)->lineText + strlen(line) - 1) = '\0';
		(str + c)->lineNumber = c + 1;
		c++;
		if(c >= lines) {
			lines += 256;
			str = (lineInfo*) realloc (str, sizeof(lineInfo) * lines);
		}
		free(line);
	}
	(str + c)->lineText = "\0";
	fclose (fp);
	return str;
}

/* Makes a copy of the lines */
lineInfo* clone (lineInfo* line) {
	lineInfo* str = (lineInfo*) malloc (sizeof(lineInfo) * size(line));
	int c = 0;
	while (strcmp((line + c)->lineText, "\0") != 0) {
		(str + c)->lineText = (char*) malloc (sizeof(char) * 256);
		strcpy ((str + c)->lineText, (line + c)->lineText);
		(str + c)->lineNumber = (line + c)->lineNumber;
		c++;
	}
	(str + c)->lineText = "\0";
	return str;
}

/* Destroys the lines once they are used */
void destroylines (lineInfo* line) {
	int c = 0;
	while(strcmp((line + c)->lineText, "\0") != 0) {
		free(line[c].lineText);
		c++;
	}
	free(line);
	line = NULL;
}

//Checks for file
int fcheck (char* path) {
	FILE* fp;
	fp = fopen(path, "r");
	if(fp == NULL)
		return 0;
	fclose(fp);
	return 1;
}

/* Returns the larger of two numbers */
int max (int n1, int n2) {
	int larger = (n1 > n2) ? n1 : n2;
	return larger;
}

/* Removes any spaces */
void removeSpaces (char* str) {
	char* d = str;
	do {
		while (*d == ' ' || *d == '\t')
			++d;
	} while ((*str++ = *d++));
}

void noSpace (lineInfo* line) {
	int c = 0;
	while (strcmp((line + c)->lineText, "\0") != 0) {
		removeSpaces ((line + c)->lineText);
		c++;
	}
}

/*Removes extra whitespace */
void removeExtraSpaces (char* str) {
	int i, x;
	for(i = x = 0; str[i]; i++) {
		if(str[i] == '\t')
			str[i] = ' ';
		if(str[i] != ' ' || (i > 0 && str[i - 1] != ' '))
			str[x++] = str[i];
	}
	str[x] = '\0';
}

void noExtraSpace (lineInfo* line) {
	int c = 0;
	while(strcmp((line + c)->lineText, "\0") != 0) {
		removeExtraSpaces ((line + c)->lineText);
		c++;
	}
}

//Replaces tabs with spaces till tab stops
char* replaceTabs (char* str) {
	int tabby (int p) {
		int s;
		s = 8 - (p % 8);
		return s;
	}
	char *temp;
	int i, j, cnt = 0, s;
	for (i = 0; str[i] != '\0'; i++)
		if (strstr(&str[i], "\t") == &str[i])
			cnt++;
	temp = (char*) malloc (256);
	i = 0;
	while (*str) {
		if(strstr(str, "\t") == str) {
			s = tabby (i);
			for(j = 0; j < s; j++)
				temp[i++] = ' ';
		}
		else
			temp[i++] = *str;
		str++;
	}
	temp[i] = '\0';
	return temp;
}

void tabsToSpaces (lineInfo* line) {
	int c = 0;
	while(strcmp((line + c)->lineText, "\0") != 0) {
		(line + c)->lineText = replaceTabs ((line + c)->lineText);
		c++;
	}
}


//Checks if file is binary by using file utility
int isBinary (char* file) {
	FILE* fp;
	char res[1024], command[100];
	strcpy(command, "/bin/file ");
	strcat(command, file);
	fp = popen(command, "r");
	if(fp == NULL)
		exit(0);
	fgets(res, sizeof(res), fp);
	if(strstr(res, "ASCII text"))
		return 0;
	return 1;	
}
