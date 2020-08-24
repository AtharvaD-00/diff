//Finds the LCS using Myers' Diff algorithm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

extern int iFlag, wFlag, bFlag;

/*
 * Creates the matrix of subsequence lengths
 * and returns the length of the longest subsequence
 */
data createMatrix (lineInfo* str1, lineInfo* str2, int len1, int len2) {
	if (wFlag) {
		noSpace(str1);
		noSpace(str2);
	}
	else if (bFlag) {
		noExtraSpace(str1);
		noExtraSpace(str2);
	}
	int i, j;
	data d;
	d.matrix = (int**) malloc (sizeof(int *) * len1);
	for (i = 0; i < len1; i++)
		d.matrix[i] = (int*) calloc (sizeof(int), len2);
	for (i = 0; i < len1 - 1; i++) {
		for (j = 0; j < len2 - 1; j++) {
			int test = iFlag ? strcasecmp(str1[i].lineText, str2[j].lineText) : strcmp(str1[i].lineText, str2[j].lineText);
			if(!test)
				d.matrix[i + 1][j + 1] = d.matrix[i][j] + 1;
			else
				d.matrix[i + 1][j + 1] = max (d.matrix[i][j + 1], d.matrix[i + 1][j]);
		}
	}
	d.length = d.matrix[len1 - 1][len2 - 1];
	return d;
}

/*
 * Returns the LCS based on a series of comparisons and the length returned by the 'createMatrix' function
 */
lineInfo* construct (data d, lineInfo* str1, lineInfo* str2, int len1, int len2) {
	int i = len1, j = len2, test;
	lineInfo* common = (lineInfo*) malloc (sizeof(lineInfo) * (d.length + 1));
	common[d.length].lineText = NULL;
	while (i > 0 && j > 0) {
		test = iFlag ? strcasecmp(str1[i - 1].lineText, str2[j - 1].lineText) : strcmp(str1[i - 1].lineText, str2[j - 1].lineText);
		if (!test) {
			d.length--;
			common[d.length].lineText = (char*) malloc (sizeof(char) * (strlen(str1[i - 1].lineText) + 1));;
			strcpy (common[d.length].lineText, str1[i - 1].lineText);
			common[d.length].lineNumber = str1[i - 1].lineNumber;
			i--;
			j--;
		}
		else {
			int temp = max (d.matrix[i - 1][j], d.matrix[i][j - 1]);
			if (temp == d.matrix[i - 1][j])
				i--;
			else
				j--;
		}
	} 
	return common;
}

/* Calls a series of functions to find the LCS */
lineInfo* subsequence (lineInfo* str1, lineInfo* str2) {
	int len1 = size(str1), len2 = size(str2);
	data lengths;
	lengths = createMatrix (str1, str2, len1 + 1, len2 + 1);
	lineInfo* sub = construct (lengths, str1, str2, len1, len2);
	return sub;
}

/* Finds the number of lines in the file */
int subsize (lineInfo* line) {
	int i = 0;
	while (line[i].lineText != NULL)
		i++;
	return i;
}
