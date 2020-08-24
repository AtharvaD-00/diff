/* Used to make a backup of the file before patching it */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "hpatch.h"

/* The copy function copies all text from one file to a copy with a .orig extension*/
void copy (char* file) {
	char* newfile = (char*) malloc (strlen(file) + 1);
	strcpy(newfile, file);
	strcat(newfile, ".orig");
	int fdr, fdw;
	char ch;
	fdr = open (file, O_RDONLY);
	fdw = open (newfile, O_WRONLY | O_CREAT, S_IRWXU);
	while(read(fdr, &ch, 1))
		write(fdw, &ch, 1);
	close(fdr);
	close(fdw);
}
