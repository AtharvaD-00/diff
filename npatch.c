#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hpatch.h"

//Patches the files by modifying the file contents according to the passed diff
void npatch (contents* str, char* pfile) {
	int q = length(str);
	FILE* fp = fopen(pfile, "r");
	size_t size = 256;
	char* line = (char*) malloc (256);
	while((getline(&line, &size, fp)) != -1) {
		line[strlen(line) - 1] = '\0';
		char *part = (char*) malloc (256), *add = (char*) malloc (256), *num = (char*) malloc (2);
		strcpy(part, line);
		strtok(part, "a");
		//For additions
		if(strcmp(part, line) != 0) {
			int a = atoi(part) + 1;
			int temp = a;
			do {
				if(getline(&add, &size, fp) == -1)
					goto end;
				add[strlen(add) - 1] = '\0';
				if(add[0] == '>') {
					if(temp == a)
						insertString (str, add + 2, a, 0);
					else
						insertString (str, add + 2, q + a, 1);
				}
				a++;
			}
			while(add[0] == '>');
			fseek(fp, - strlen(add) - 1, SEEK_CUR);
		}
		strcpy(part, line);
		strtok(part, "d");
		//For deletions
		if(strcmp(part, line) != 0) {
			int d1, d2;
			num = strtok (part, ",");
			d1 = atoi(num);
			num = strtok (NULL, ",");
			d2 = num ? atoi(num) : d1;
			while(d1 <= d2) {
				getline(&add, &size, fp);
				add[strlen(add) - 1] = '\0';
				if(add[0] == '<' && strcmp(add + 2, string(str, d1)) == 0)
					deleteString (str, d1);
				d1++;
			}
		}
		strcpy(part, line);
		strtok(part, "c");
		//For changes
		if(strcmp(part, line) != 0) {
			int d1, d2, a, temp;
			num = strtok (part, ",");
			temp = a = d1 = atoi(num);
			num = strtok (NULL, ",");
			d2 = num ? atoi(num) : d1;
			while(d1 <= d2) {
				getline(&add, &size, fp);
				add[strlen(add) - 1] = '\0';
				if(add[0] == '<' && strcmp(add + 2, string(str, d1)) == 0)
					deleteString (str, d1);
				d1++;
			}
			getline(&add, &size, fp);
			if(strcmp(add, "---\n") != 0)
				goto end;
			do {
				if(getline(&add, &size, fp) == -1)
					goto end;
				add[strlen(add) - 1] = '\0';
				if(add[0] == '>') {
					if(temp == a)
						insertString (str, add + 2, a, 0);
					else
						insertString (str, add + 2, q + a, 1);
				}
				a++;
			}
			while(add[0] == '>');
			fseek(fp, - strlen(add) - 1, SEEK_CUR);
		}
	}
	end:
	return;
}
