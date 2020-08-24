/* Queue functions responsible for printing the diff */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "queue.h"

//Global variables
int aflag = 0, dflag = 0, cflag = 0;
static int a = 0, b = 0;
//Flag variables from main file
extern int colorFlag, tFlag;

//Traverse for normal diff
void qtraverse (queue q) {
	node* ptr = q.head;
	while(ptr) {
		if (ptr->o.op == 'd')
			dflag = 1;
		if (ptr->o.op == 'a')
			aflag = 1;
		ptr = ptr->next;
	}
	if (aflag == 1 && dflag == 1)
		cflag = 1;
}

//Print normal diff
void qprint (queue* q, lineInfo* str1, lineInfo* str2) {
	operation o;
	int add[qlength(q)], del[qlength(q)], a = 0, d = 0, i;
	if (cflag == 1) {
		int temp = qlength(q) - 1;
		while(qpeek(q) != 'a') {
			o = deq(q);
			if (qlength(q) == temp) {
				if (colorFlag)
					printf("\033[0;36m%d,\033[0m", o.x);
				else
					printf("%d,", o.x);
			}
			del[d] = o.x;
			d++;
		}
		o = deq(q);
		if(colorFlag)
			printf("\033[0;36m%dc\033[0m", o.x);
		else
			printf("%dc", o.x);
		del[d] = o.x;
		d++;
		o = deq(q);
		if(colorFlag)
			printf("\033[0;36m%d\033[0m", o.y);
		else
			printf("%d", o.y);
		add[a] = o.y;
		a++;
		while(!qempty(*q)) {
			o = deq(q);
			if(qempty(*q)) {
				if(colorFlag)
					printf("\033[0;36m,%d\033[0m", o.y);
				else
					printf(",%d", o.y);
			}
			add[a] = o.y;
			a++;
		}
		printf("\n");
		for(i = 0; i < d; i++)
			if(colorFlag)
				printf("\033[0;31m< %s\033[0m\n", str1[del[i] - 1].lineText);
			else
				printf("< %s\n", str1[del[i] - 1].lineText);
		printf("---\n");
		for(i = 0; i < a; i++)
			if(colorFlag)
				printf("\033[0;32m> %s\033[0m\n", str2[add[i] - 1].lineText);
			else
				printf("> %s\n", str2[add[i] - 1].lineText);
		aflag = dflag = cflag = 0;
		return;
	}
	if (aflag == 1) {
		o = deq(q);
		if(colorFlag)
			printf("\033[0;36m%da%d\033[0m", o.x, o.y);
		else
			printf("%da%d", o.x, o.y);
		add[a] = o.y;
		a++;
		while(qlength(q) != 0) {
			o = deq(q);
			if (qlength(q) == 0) {
				if(colorFlag)
					printf("\033[0;36m,%d\033[0m", o.y);
				else
					printf(",%d", o.y);
			}
			add[a] = o.y;
			a++;
		}
		printf("\n");
		for(i = 0; i < a; i++)
			if(colorFlag)
				printf("\033[0;32m> %s\033[0m\n", str2[add[i] - 1].lineText);
			else
				printf("> %s\n", str2[add[i] - 1].lineText);
		aflag = dflag = cflag = 0;
		return;
	}
	if (dflag == 1) {
		int temp = qlength(q) - 1;
		while(qlength(q) != 1) {
			o = deq(q);
			if (qlength(q) == temp) {
				if(colorFlag)
					printf("\033[0;36m%d,\033[0m", o.x);
				else
					printf("%d,", o.x);
			}
			del[d] = o.x;
			d++;
		}
		o = deq(q);
		if(colorFlag)
			printf("\033[0;36m%dd%d\033[0m\n", o.x, o.y);
		else
			printf("%dd%d\n", o.x, o.y);
		del[d] = o.x;
		d++;
		for(i = 0; i < d; i++)
			if(colorFlag)
				printf("\033[0;31m< %s\033[0m\n", str1[del[i] - 1].lineText);
			else
				printf("< %s\n", str1[del[i] - 1].lineText);
		aflag = dflag = cflag = 0;
		return;
	}
}

//Traverse for y diff
void qytraverse (queue q) {
	node* ptr = q.head;
	while(ptr) {
		if (ptr->o.op == 'd')
			dflag++;
		if (ptr->o.op == 'a')
			aflag++;
		ptr = ptr->next;
	}
	if (aflag != 0 && dflag != 0)
		cflag = 1;
}

//Print for y diff
void qyprint (queue* q, lineInfo* str1, lineInfo* str2) {
	operation o;
	char* str = (char*) malloc (256);
	strcpy(str, "");
	if (cflag == 1) {
		int temp = 61 + (2 * tFlag), y, i, flag;
		queue qtemp;
		qinit(&qtemp);
		while(q->head->o.op == 'd') {
			o = deq(q);
			if(qempty(qtemp))
				while (a < o.x && b < o.y) {
					if (tFlag) {
						strcpy(str, str1[a].lineText);
						while(strlen(str) < temp)
							strcat(str, " ");
						printf("%-.*s    ", temp, str);
						strcpy(str, str2[b].lineText);
						while(strlen(str) < temp)
							strcat(str, " ");
						printf("%-.*s\n", temp, str);
						strcpy(str, "");
					}
					else {
						strcat(str, str1[a].lineText);
						y = temp - tabs(str);
						flag = strlen(str) + tabs(str) > temp ? 1 : 0;
						while(strlen(str) + tabs(str) <= temp)
							strcat(str, "\t");
						printf("%-.*s", y, str);
						if(flag)
							printf("   ");
						strcpy(str, "");
						strcat(str, str2[b].lineText);
						y = temp - tabs(str2[b].lineText);
						printf("%-.*s\n", y, str);
						strcpy(str, "");
					}
					a++;
					b++;
				}
			enq(&qtemp, o);
		}
		while(aflag && dflag) {
			o = deq(&qtemp);
			if (tFlag) {
				strcpy(str, str1[o.x - 1].lineText);
				while(strlen(str) < temp)
					strcat(str, " ");
				printf("%-.*s |  ", temp, str);
				strcpy(str, "");
			}
			else {
				strcat(str, str1[o.x - 1].lineText);
				while(strlen(str) + tabs(str) <= temp - 13)
					strcat(str, "\t");
				for(i = 1; i <= 13; i++)
					strcat(str, " ");
				y = temp - tabs(str);
				printf("%-.*s", y, str);
				printf(" | ");
				strcpy(str, "");
			}
			o = deq(q);
			if (tFlag) {
				strcpy(str, str2[o.y - 1].lineText);
				while(strlen(str) < temp)
					strcat(str, " ");
				printf("%-.*s\n", temp, str);
				strcpy(str, "");
			}
			else {
				strcat(str, str2[o.y - 1].lineText);
				y = temp - tabs(str);
				while(strlen(str) + tabs(str) <= temp)
					strcat(str, "\t");
				printf("%-.*s\n", y, str);
				strcpy(str, "");
			}
			aflag--;
			dflag--;
			a = o.x;
			b = o.y;
		}
		while(dflag) {
			o = deq(&qtemp);
			if (tFlag) {
				strcpy(str, str1[o.x - 1].lineText);
				while(strlen(str) < temp)
					strcat(str, " ");
				if(colorFlag)
					printf("\033[0;31m%-.*s <\n\033[0m", temp, str);
				else
					printf("%-.*s <\n", temp, str);
				strcpy(str, "");
			}
			else {
				strcat(str, str1[o.x - 1].lineText);
				while(strlen(str) + tabs(str) <= temp - 13)
					strcat(str, "\t");
				for(i = 1; i <= 13; i++)
					strcat(str, " ");
				y = temp - tabs(str);
				if(colorFlag)
					printf("\033[0;31m%-.*s\033[0m", y, str);
				else
					printf("%-.*s", y, str);
				if(colorFlag)
					printf("\033[0;31m <\033[0m\n");
				else
					printf(" <\n");
				strcpy(str, "");
			}
			dflag--;
		}
		while(aflag) {
			o = deq(q);
			if (tFlag) {
				for(i = 0; i < temp; i++)
					strcat(str, " ");
				if(colorFlag)
					printf("\033[0;32m%-.*s >  \033[0m", temp, str);
				else
					printf("%-.*s >  ", temp, str);
				strcpy(str, str2[o.y - 1].lineText);
				while(strlen(str) < temp)
					strcat(str, " ");
				if(colorFlag)
					printf("\033[0;32m%-.*s\033[0m\n", temp, str);
				else
					printf("%-.*s\n", temp, str);
				strcpy(str, "");
			}
			else {
				y = temp - tabs(str);
				flag = strlen(str) + tabs(str) > temp ? 0 : 1;
				while(strlen(str) + tabs(str) <= temp - 6)
					strcat(str, "\t");
				printf("%-.*s", y, str);
				if(flag)
					printf("     ");
				if(colorFlag)
					printf("\033[0;32m > \033[0m");
				else
					printf(" > ");
				strcpy(str, "");
				strcat(str, str2[o.y - 1].lineText);
				y = temp - tabs(str);
				while(strlen(str) + tabs(str) <= temp)
					strcat(str, "\t");
				if(colorFlag)
					printf("\033[0;32m%-.*s\033[0m\n", y, str);
				else
					printf("%-.*s\n", y, str);
				strcpy(str, "");
			}
			aflag--;
			a = o.x;
			b = o.y;
		}
		aflag = dflag = cflag = 0;
		return;
	}
	if (aflag != 0) {
		int temp = 61 + (2 * tFlag), y, i, flag, len = qlength(q) - 1;
		while(qlength(q) != 0) {
			o = deq(q);
			if(qlength(q) == len)
				while (a < o.x && b < o.y) {
					if (tFlag) {
						strcpy(str, str1[a].lineText);
						while(strlen(str) < temp)
							strcat(str, " ");
						printf("%-.*s    ", temp, str);
						strcpy(str, str2[b].lineText);
						while(strlen(str) < temp)
							strcat(str, " ");
						printf("%-.*s\n", temp, str);
						strcpy(str, "");
					}
					else {
						strcat(str, str1[a].lineText);
						y = temp - tabs(str);
						flag = strlen(str) + tabs(str) > temp ? 1 : 0;
						while(strlen(str) + tabs(str) <= temp)
							strcat(str, "\t");
						printf("%-.*s", y, str);
						if(flag)
							printf("   ");
						strcpy(str, "");
						strcat(str, str2[b].lineText);
						y = temp - tabs(str2[b].lineText);
						printf("%-.*s\n", y, str);
						strcpy(str, "");
					}
					a++;
					b++;
				}
			if (tFlag) {
				for(i = 0; i < temp; i++)
					strcat(str, " ");
				if(colorFlag)
					printf("\033[0;32m%-.*s >  \033[0m", temp, str);
				else
					printf("%-.*s >  ", temp, str);
				strcpy(str, str2[o.y - 1].lineText);
				while(strlen(str) < temp)
					strcat(str, " ");
				if(colorFlag)
					printf("\033[0;32m%-.*s\033[0m\n", temp, str);
				else
					printf("%-.*s\n", temp, str);
				strcpy(str, "");
			}
			else {
				y = temp - tabs(str);
				flag = strlen(str) + tabs(str) > temp ? 0 : 1;
				while(strlen(str) + tabs(str) <= temp - 6)
					strcat(str, "\t");
				printf("%-.*s", y, str);
				if(flag)
					printf("     ");
				if(colorFlag)
					printf("\033[0;32m > \033[0m");
				else
					printf(" > ");
				strcpy(str, "");
				strcat(str, str2[o.y - 1].lineText);
				y = temp - tabs(str);
				while(strlen(str) + tabs(str) <= temp)
					strcat(str, "\t");
				if(colorFlag)
					printf("\033[0;32m%-.*s\033[0m\n", y, str);
				else
					printf("%-.*s\n", y, str);
				strcpy(str, "");
			}
		}
		a = o.x;
		b = o.y;
		aflag = dflag = cflag = 0;
		return;
	}
	if (dflag != 0) {
		int temp = 61 + (2 * tFlag), y, flag, len = qlength(q) - 1, i;
		while(qlength(q) != 0) {
			o = deq(q);
			if(qlength(q) == len)
				while (a < o.x && b < o.y) {
					if (tFlag) {
						strcpy(str, str1[a].lineText);
						while(strlen(str) < temp)
							strcat(str, " ");
						printf("%-.*s    ", temp, str);
						strcpy(str, str2[b].lineText);
						while(strlen(str) < temp)
							strcat(str, " ");
						printf("%-.*s\n", temp, str);
						strcpy(str, "");
					}
					else {
						strcat(str, str1[a].lineText);
						y = temp - tabs(str);
						flag = strlen(str) + tabs(str) > temp ? 1 : 0;
						while(strlen(str) + tabs(str) <= temp)
							strcat(str, "\t");
						printf("%-.*s", y, str);
						if(flag)
							printf("   ");
						strcpy(str, "");
						strcat(str, str2[b].lineText);
						y = temp - tabs(str2[b].lineText);
						printf("%-.*s\n", y, str);
						strcpy(str, "");
					}
					a++;
					b++;
				}
			if (tFlag) {
				strcpy(str, str1[o.x - 1].lineText);
				while(strlen(str) < temp)
					strcat(str, " ");
				if(colorFlag)
					printf("\033[0;31m%-.*s <\n\033[0m", temp, str);
				else
					printf("%-.*s <\n", temp, str);
				strcpy(str, "");
			}
			else {
				strcat(str, str1[o.x - 1].lineText);
				while(strlen(str) + tabs(str) <= temp - 13)
					strcat(str, "\t");
				for(i = 1; i <= 13; i++)
					strcat(str, " ");
				y = temp - tabs(str);
				if(colorFlag)
					printf("\033[0;31m%-.*s\033[0m", y, str);
				else
					printf("%-.*s", y, str);
				if(colorFlag)
					printf("\033[0;31m <\033[0m\n");
				else
					printf(" <\n");
				strcpy(str, "");
			}
		}
		a = o.x;
		b = o.y;
		aflag = dflag = cflag = 0;
		return;
	}
}

//Print any remaining lines in y diff
void remprint (lineInfo* str1, lineInfo* str2) {
	int temp = 61 + (2 * tFlag), y, flag, m, n;
	m = size(str1);
	n = size(str2);
	char* str = (char*) malloc (256);
	strcpy(str, "");
	while(a < m && b < n) {
		if (tFlag) {
			strcpy(str, str1[a].lineText);
			while(strlen(str) < temp)
				strcat(str, " ");
			printf("%-.*s    ", temp, str);
			strcpy(str, str2[b].lineText);
			while(strlen(str) < temp)
				strcat(str, " ");
			printf("%-.*s\n", temp, str);
			strcpy(str, "");
		}
		else {
			strcat(str, str1[a].lineText);
			y = temp - tabs(str);
			flag = strlen(str) + tabs(str) > temp ? 1 : 0;
			while(strlen(str) + tabs(str) <= temp)
				strcat(str, "\t");
			printf("%-.*s", y, str);
			if(flag)
				printf("   ");
			strcpy(str, "");
			strcat(str, str2[b].lineText);
			y = temp - tabs(str2[b].lineText);
			printf("%-.*s\n", y, str);
			strcpy(str, "");
		}
		a++;
		b++;
	}
	while(a < m) {
		if (tFlag) {
			strcpy(str, str1[a].lineText);
			while(strlen(str) < temp)
				strcat(str, " ");
			printf("%-.*s\n", temp, str);
			strcpy(str, "");
		}
		else {
			strcat(str, str1[a].lineText);
			y = temp - tabs(str);
			while(strlen(str) + tabs(str) <= temp)
				strcat(str, "\t");
			printf("%-.*s", y, str);
			strcpy(str, "");
		}
		a++;
	}
	while(b < n) {
		if (tFlag) {
			while(strlen(str) < temp)
				strcat(str, " ");
			printf("%-.*s    ", temp, str);
			strcpy(str, str2[b].lineText);
			while(strlen(str) < temp)
				strcat(str, " ");
			printf("%-.*s\n", temp, str);
			strcpy(str, "");
		}
		else {
			y = temp - tabs(str);
			flag = strlen(str) + tabs(str) > temp ? 1 : 0;
			while(strlen(str) + tabs(str) <= temp)
				strcat(str, "\t");
			printf("%-.*s", y, str);
			if(flag)
				printf("   ");
			strcpy(str, "");
			strcat(str, str2[b].lineText);
			y = temp - tabs(str2[b].lineText);
			printf("%-.*s\n", y, str);
			strcpy(str, "");
		}
		b++;
	}
}

//Traverse for c diff
void qctraverse (queue q) {
	node* ptr = q.head;
	while(ptr) {
		if (ptr->o.op == 'd')
			dflag++;
		if (ptr->o.op == 'a')
			aflag++;
		ptr = ptr->next;
	}
	if (aflag != 0 && dflag != 0)
		cflag = 1;
}

//Print for c diff	
void qcprint (queue* q, lineInfo* str1, lineInfo* str2, char* print1, char* print2) {
	operation o;
	if (cflag == 1) {
		queue qtemp;
		qinit(&qtemp);
		while(q->head->o.op == 'd') {
			o = deq(q);
			if(qempty(qtemp))
				while (a < o.x && b < o.y) {
					print1[a] = ' ';
					print2[b] = ' ';
					a++;
					b++;
				}
			enq(&qtemp, o);
		}
		while(aflag && dflag) {
			o = deq(&qtemp);
			print1[o.x - 1] = '!';
			o = deq(q);
			print2[o.y - 1] = '!';
			aflag--;
			dflag--;
			a = o.x;
			b = o.y;
		}
		while(dflag) {
			o = deq(&qtemp);
			print1[o.x - 1] = '-';
			print2[o.y - 1] = ' ';
			dflag--;
		}
		while(aflag) {
			o = deq(q);
			print1[o.x - 1] = ' ';
			print2[o.y - 1] = '+';
			aflag--;
			a = o.x;
			b = o.y;
		}
		aflag = dflag = cflag = 0;
		return;
	}
	if (aflag != 0) {
		int len = qlength(q) - 1;
		while(qlength(q) != 0) {
			o = deq(q);
			if(qlength(q) == len)
				while (a < o.x && b < o.y) {
					print1[a] = ' ';
					print2[b] = ' ';
					a++;
					b++;
				}
			print1[o.x - 1] = ' ';
			print2[o.y - 1] = '+';
		}
		a = o.x;
		b = o.y;
		aflag = dflag = cflag = 0;
		return;
	}
	if (dflag != 0) {
		int len = qlength(q) - 1;
		while(qlength(q) != 0) {
			o = deq(q);
			if(qlength(q) == len)
				while (a < o.x && b < o.y) {
					print1[a] = ' ';
					print2[b] = ' ';
					a++;
					b++;
				}
			print1[o.x - 1] = '-';
			print2[o.y - 1] = ' ';
			dflag--;
		}
		a = o.x;
		b = o.y;
		aflag = dflag = cflag = 0;
		return;
	}
}

void printcdiff (lineInfo* str1, lineInfo* str2, char* print1, char* print2) {
	int m, n, i = 0, j = 0, k, x1 = 0, y1 = 0, x2 = 1, y2 = 1, flag1, flag2, t1, t2;
	m = size(str1);
	n = size(str2);
	while (a < m)
		print1[a++] = ' ';
	while (b < n)
		print2[b++] = ' ';
	start:
	flag1 = 0, flag2 = 0;
	for (; print1[i] == ' ' && print2[j] == ' '; i++, j++);
	t1 = print1[i] == ' ';
	t2 = print2[j] == ' ';
	if(t1)
		j++;
	if(t2)
		i++;
	if(i == m && j == n)
		goto end;
	x1 = i - 2 < x2 ? x2 : i - 2;
	y1 = j - 2 < y2 ? y2 : j - 2;
	while(flag1 < 6) {
		flag1 = 0;
		for (k = i; k < i + 6; k++) {
			if (print1[k] == ' ')
				flag1++;
			if (print1[k] == 'n')
				goto f1;
		}
		i++;
	}
	f1:
	i += 5;
	x2 = i >= m ? m : i - 3;
	while(flag2 < 6) {
		flag2 = 0;
		for (k = j; k < j + 6; k++) {
			if (print2[k] == ' ')
				flag2++;
			if (print2[k] == 'n')
				goto f2;
		}
		j++;
	}
	f2:
	j += 5;
	y2 = j >= n ? n : j - 3;
	int c1, c2;
	for(k = x1; k <= x2; k++) {
		if(print1[k - 1] == '!')
			c1 = 1;
		if(print1[k - 1] == 'n')
			print1[k - 1] = ' ';
	}
	for(k = x1; k <= x2; k++) {
		if(print2[k - 1] == '!')
			c2 = 1;
		if(print2[k - 1] == 'n')
			print2[k - 1] = ' ';
	}
	if(colorFlag)
		printf("\033[0;36m*** %d,%d ****\033[0m\n", x1, x2);
	else
		printf("*** %d,%d ****\n", x1, x2);
	if (((x1 != 1 && y1 != 1 && x2 != m && y2 != n) || y2 < x2 || y1 > x1 || c1)) {
		for(k = x1; k <= x2; k++) {
			if(colorFlag)
				printf("\033[0;31m%c %s\033[0m\n", print1[k - 1], str1[k - 1].lineText);
			else
				printf("%c %s\n", print1[k - 1], str1[k - 1].lineText);
		}
	}
	if(colorFlag)
		printf("\033[0;36m--- %d,%d ----\033[0m\n", y1, y2);
	else
		printf("--- %d,%d ----\n", y1, y2);
	if (((x1 != 1 && y1 != 1 && x2 != m && y2 != n) || y2 > x2 || y1 < x1 || c2)) {
		for(k = y1; k <= y2; k++) {
			if(colorFlag)
				printf("\033[0;32m%c %s\033[0m\n", print2[k - 1], str2[k - 1].lineText);
			else
				printf("%c %s\n", print2[k - 1], str2[k - 1].lineText);
		}
	}
	if(i < m || j < n) {
		if(colorFlag)
			printf("\033[0;36m***************\033[0m\n");
		else
			printf("***************\n");
		goto start;
	}
	end:
	strcpy(print1, "");
	strcpy(print2, "");
}

//Counts the number of tabs
int tabs (char* str) {
	int t = 0, i = 0, column = 0;
	while(str[i] != '\0' && i + t <= 61 + (2 * tFlag)) {
		if(str[i] == '\t') {
			t += 7 - (column % 8);
			column += 7 - (column % 8);
		}
		column++;
		i++;
	}
	return t;
}
