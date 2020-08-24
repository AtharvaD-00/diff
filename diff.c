/* Main file of the program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "header.h"

#define FILE 0
#define DIRECTORY 1

//Flag variables
int yFlag = 0, cFlag = 0, iFlag = 0, bFlag = 0, wFlag = 0, tFlag = 0, rFlag = 0, colorFlag = 0;

int main (int argc, char* argv[]) {
	// You cannot have less than the minimum of 3 command line arguments!
	if (argc < 3) {
		errno = EINVAL;
		perror("Usage : ./diff [options] <file 1> <file 2>");
		return errno;
	}
	char opt;
	int c = 0, flag = 0, i, j, targc = argc;
	char* targv[argc];
	//Copy all the command line arguments for further use, if needed
	for(i = 0; i < argc; i++) {
		targv[i] = (char*) malloc (strlen(argv[i] + 1));
		strcpy(targv[i], argv[i]);
	}
	/* Check for the --color flag */
	for(i = 1; i < argc; i++)
		if(strcmp(argv[i], "--color") == 0) {
			colorFlag = 1;
			for(j = i; j < argc - 1; j++)
				argv[j] = argv[j + 1];
			argc--;
		}
	/* Get the command line options */
	while((opt = getopt(argc, argv, ":bcitwyr")) != -1) {
		flag = set(opt);
		if(flag) {
			printf("./diff: invalid option -- '%c'\n", optopt);
			return 0;
		}
	}
	char *path1, *path2;
	/* Check for entered paths and extra arguments */
	for (; optind < argc; optind++) {
		if (c == 0) {
			path1 = (char*) malloc (sizeof(char) * (strlen(argv[optind] + 1)));
			strcpy(path1, argv[optind]); 
		}
		else if (c == 1) {
			path2 = (char*) malloc (sizeof(char) * (strlen(argv[optind] + 1)));
			strcpy(path2, argv[optind]); 
		}
		else {
			printf("./diff: %s: extra operand\n", argv[optind]);
			return 0;
		}
		c++;
	}
	//c and y are not allowed together
	if(cFlag && yFlag) {
		printf("diff: conflicting output style options\n");
		return 0;
	}
	//Same paths correspond to same files
	if (strcmp(path1, path2) == 0)
		return 0;
	/* Determine if the paths represent files or directories, and take appropriate actions */
	int t1, t2;
	if(dcheck(path1))
		t1 = DIRECTORY;
	else if(fcheck(path1))
		t1 = FILE;
	else {
		printf("./diff: %s: No such file or directory\n", path1);
		t1 = -1;
	}
	if(dcheck(path2))
		t2 = DIRECTORY;
	else if(fcheck(path2))
		t2 = FILE;
	else {
		printf("./diff: %s: No such file or directory\n", path2);
		t2 = -1;
	}
	if(t1 == -1 || t2 == -1)
		return 0;
	if(t1 && t2)
		files(path1, path2, targc, targv);
	else if(!(t1 || t2)) {
		// Call the 'lines' function to store the files line by line in contiguous lineInfo pointers
		lineInfo *str1, *str2, *temp1, *temp2;
		str1 = lines(path1);
		str2 = lines(path2);
		//Make a copy
		temp1 = clone(str1);
		temp2 = clone(str2);
		lineInfo* sub = subsequence(temp1, temp2);
		int s = subsize(sub);
		//See if LCS is equal to the two files' contents
		if (s == size(str1) && s == size(str2))
			return 0;
		//Check for binary file
		if(isBinary(path1) || isBinary(path2)) {
			printf("Binary files %s and %s differ\n", path1, path2);
			return 0;
		}
		//c flag
		if (cFlag) {
			printf("*** %s\t", path1);
			printFileProperties(path1);
			printf("--- %s\t", path2);
			printFileProperties(path2);
			printf("***************\n");
		}
		editMap ed = backtrack(temp1, temp2);
		destroylines(temp1);
		destroylines(temp2);
		createDiff (ed, str1, str2);
		destroylines(str1);
		destroylines(str2);
	}
	else if (!t1 && t2)
		onedir(path1, path2, 1);
	else if (t1 && !t2)
		onedir(path1, path2, 0);
	return 0;
}

/* Sets flags as input by the user */
int set (char ch) {
	switch(ch) {
		case 'y': yFlag = 1;
			return 0;
			break;
		case 'c': cFlag = 1;
			return 0;
			break;
		case 'i': iFlag = 1;
			return 0;
			break;
		case 'w': wFlag = 1;
			return 0;
			break;
		case 'b': bFlag = 1;
			return 0;
			break;
		case 't': tFlag = 1;
			return 0;
			break;
		case 'r': rFlag = 1;
			return 0;
			break;
		default: return 1;
			break;
	}
}
