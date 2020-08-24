/* Recursive diff */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "header.h"

//Flag variables from main file
extern int cFlag, rFlag;

void files (char* path1, char* path2, int c, char* a[]) {
	lineInfo *f1, *f2, *common;
	char str1[1000], str2[1000];
	//Get all the files in the directories
	f1 = listFiles(path1, 0);
	f2 = listFiles(path2, 0);
	//Find the common files and subdirectories
	common = subsequence(f1, f2);
	int x = 0, y = 0, i = 0;
	while (common[i].lineText != NULL) {
		// For matches
		start:
		if(common[i].lineText == NULL)
			break;
		//Check for common entities
		if (strcmp(f1[x].lineText, common[i].lineText) == 0 && strcmp(f2[y].lineText, common[i].lineText) == 0) {
			if (f1[x].lineNumber + f2[y].lineNumber) {
				strcpy(str1, path1);
				strcat(str1, "/");
				strcat(str1, common[i].lineText);
				strcpy(str2, path2);
				strcat(str2, "/");
				strcat(str2, common[i].lineText);
				if(rFlag)
					files(str1, str2, c, a);
				else
					printf("Common subdirectories: %s and %s\n", str1, str2);
			}
			else {
				//The same operations as for a normal call to diff
				lineInfo *str1, *str2, *temp1, *temp2;
				char file1[1000], file2[1000];
				strcpy(file1, path1);
				strcat(file1, "/");
				strcat(file1, common[i].lineText);
				strcpy(file2, path2);
				strcat(file2, "/");
				strcat(file2, common[i].lineText);
				int t1, t2;
				t1 = strstr(file1, " ") ? 1 : 0;
				t2 = strstr(file2, " ") ? 1 : 0;
				str1 = lines(file1);
				str2 = lines(file2);
				temp1 = clone(str1);
				temp2 = clone(str2);
				lineInfo* sub = subsequence(temp1, temp2);
				int j, s = subsize(sub);
				if (s == size(str1) && s == size(str2)) {
					x++;
					y++;
					i++;
					goto start;
				}
				if(isBinary(file1) || isBinary(file2)) {
					printf("Binary files %s and %s differ\n", file1, file2);
					x++;
					y++;
					i++;
					goto start;
				}
				for (j = 0; j < c; j++) {
					if(strcmp(a[j], path1) == 0) {
						if(t1)
							printf("\"%s \"", file1);
						else
							printf("%s ", file1);
					}
					else if(strcmp(a[j], path2) == 0) {
						if(t2)
							printf("\"%s \"", file2);
						else
							printf("%s ", file2);
					}
					else
						printf("%s ", a[j]);
				}
				printf("\n");
				if (cFlag) {
					printf("*** %s\t", file1);
					printFileProperties(file1);
					printf("--- %s\t", file2);
					printFileProperties(file2);
					printf("***************\n");
				}
				editMap ed = backtrack(temp1, temp2);
				destroylines(temp1);
				destroylines(temp2);
				createDiff (ed, str1, str2);
				destroylines(str1);
				destroylines(str2);
			}
			x++;
			y++;
			i++;
		}
		else if (strcmp(f1[x].lineText, common[i].lineText) == 0 && strcmp(f2[y].lineText, common[i].lineText) != 0) {
			printf("Only in %s: %s\n", path2, f2[y].lineText);
			y++;
		}
		else if (strcmp(f1[x].lineText, common[i].lineText) != 0 && strcmp(f2[y].lineText, common[i].lineText) == 0) {
			printf("Only in %s: %s\n", path1, f1[x].lineText);
			x++;
		}
		//Moves it alphabetically
		else if (strcmp(f1[x].lineText, common[i].lineText) != 0 && strcmp(f2[y].lineText, common[i].lineText) != 0) {
			if(strcmp(f1[x].lineText, f2[y].lineText) < 0) {
				printf("Only in %s: %s\n", path1, f1[x].lineText);
				x++;
			}
			else if(strcmp(f1[x].lineText, f2[y].lineText) > 0) {
				printf("Only in %s: %s\n", path2, f2[y].lineText);
				y++;
			}
		}
	}
	while (strcmp(f1[x].lineText, "\0")) {
		printf("Only in %s: %s\n", path1, f1[x].lineText);
		x++;
	}
	while (strcmp(f2[y].lineText, "\0")) {
		printf("Only in %s: %s\n", path2, f2[y].lineText);
		y++;
	}
}

//For one file and one directory
void onedir (char* path1, char* path2, int choice) {
	lineInfo *f;
	char fpath[1000], dpath[1000];
	if(choice) {
		strcpy(fpath, path1);
		strcpy(dpath, path2);
	}
	else {
		strcpy(dpath, path1);
		strcpy(fpath, path2);
	}
	f = listFiles(dpath, 0);
	int i = 0;
	while(f[i].lineText != NULL) {
		if (strcmp(f[i].lineText, fpath) == 0) {
			lineInfo *str1, *str2, *temp1, *temp2;
			char file1[1000], file2[1000];
			if(choice) {
				strcpy(file1, fpath);
				strcpy(file2, dpath);
				strcat(file2, "/");
				strcat(file2, f[i].lineText);
			}
			else {
				strcpy(file1, dpath);
				strcat(file1, "/");
				strcat(file1, f[i].lineText);
				strcpy(file2, fpath);
			}
			str1 = lines(file1);
			str2 = lines(file2);
			temp1 = clone(str1);
			temp2 = clone(str2);
			lineInfo* sub = subsequence(temp1, temp2);
			int s = subsize(sub);
			if (s == size(str1) && s == size(str2))
				break;
			if (cFlag) {
				printf("*** %s\t", file1);
				printFileProperties(file1);
				printf("--- %s\t", file2);
				printFileProperties(file2);
				printf("***************\n");
			}
			editMap ed = backtrack(temp1, temp2);
			destroylines(temp1);
			destroylines(temp2);
			createDiff (ed, str1, str2);
			destroylines(str1);
			destroylines(str2);
		}
		i++;
	}
}

//Gets all the files and subdirectories in the directory
lineInfo* listFiles (char* path, int flag) {
	struct dirent **dp;
	char newpath[1000];
	lineInfo *list = (lineInfo*) malloc (sizeof(lineInfo)), *temp;
	DIR *dir = opendir(path);
	if(!dir) {
		list->lineNumber = 0;
		return list;
	}
	if(flag) {
		list->lineNumber = 1;
		return list;
	}
	int i, j, n = scandir(path, &dp, NULL, alphasort);
	for (i = 2, j = 0; i < n; i++, j++) {
		if(strcmp(dp[i]->d_name, ".") != 0 && strcmp(dp[i]->d_name, "..") != 0) {
			(list + j)->lineText = (char*) malloc (1000);
			strcpy((list + j)->lineText, dp[i]->d_name);
			strcpy(newpath, path);
			strcat(newpath, "/");
			strcat(newpath, dp[i]->d_name);
			temp = listFiles(newpath, 1);
			(list + j)->lineNumber = temp->lineNumber;
			list = (lineInfo*) realloc (list, sizeof(lineInfo) * i);
		}
	}
	(list + j)->lineText = "\0";
	closedir(dir);
	return list;
}

//Check for a directory
int dcheck (char* path) {
	DIR *dir = opendir(path);
	if(!dir)
		return 0;
	closedir(dir);
	return 1;
}
