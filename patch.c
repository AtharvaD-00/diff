//Main file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "hpatch.h"

//Flag variables
int yFlag = 0, bFlag = 0;

int main (int argc, char* argv[]) {
	// You cannot have less than the minimum of 3 command line arguments!
	if (argc < 3) {
		errno = EINVAL;
		perror("Usage : ./patch [options] <original file> <patch file>");
		return errno;
	}
	char opt;
	int c = 0, flag = 0;
	/* Get the command line options */
	while((opt = getopt(argc, argv, ":yb")) != -1) {
		flag = set(opt);
		if(flag) {
			printf("patch: invalid option -- '%c'\n", optopt);
			return 0;
		}
	}
	char *ofile, *pfile;
	/* Retrieve the file names */
	for (; optind < argc; optind++) {
		if (c == 0) {
			ofile = (char*) malloc (sizeof(char) * (strlen(argv[optind] + 1)));
			strcpy(ofile, argv[optind]); 
		}
		else if (c == 1) {
			pfile = (char*) malloc (sizeof(char) * (strlen(argv[optind] + 1)));
			strcpy(pfile, argv[optind]); 
		}
		else {
			printf("patch: %s: extra operand\n", argv[optind]);
			return 0;
		}
		c++;
	}
	if (bFlag)
		copy(ofile);
	contents* str = NULL;
	init(str);
	str = lines(ofile);
	printf("patching file %s\n", ofile);
	npatch(str, pfile);
	traverse(*str, ofile);
	return 0;
}

/* Sets flags as input by the user */
int set (char ch) {
	switch(ch) {
		case 'y': yFlag = 1;
			return 0;
			break;
		case 'b': bFlag = 1;
			return 0;
			break;
		default: return 1;
			break;
	}
}
