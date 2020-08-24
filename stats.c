/* Finding last modification date of a given file */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "header.h"

//Prints the stats if c option is entered
void printFileProperties (char* file) {
	struct stat stats;
	stat(file, &stats);
	struct tm dt;
	// File modification time
	dt = *(localtime(&stats.st_mtime));
	int offset = ((dt.tm_gmtoff / 3600) * 100) + (dt.tm_gmtoff / 60) % 60;
	char ch = offset > 0 ? '+' : '-';
	printf("%04d-%02d-%02d %02d:%02d:%02d.%-9ld %c%04d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec, stats.st_mtim.tv_nsec, ch, offset);
}
