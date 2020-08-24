/* Finding the Shortest Edit Script */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//Flag variables from main file
extern int iFlag, wFlag, bFlag;

/* Finds the path for the shortest edit script */
solution editScript (lineInfo* str1, lineInfo* str2) {
	int m, n, max, d = 0, k, i, j;
	n = size(str1);
	m = size(str2);
	max = m + n;
	int v[2 * max + 1];
	solution s;
	s.trace = (point**) malloc (sizeof(point*) * max);
	for (i = 0; i < max; i++) {
		s.trace[i] = (point*) malloc (sizeof(point) * (2 * max + 1));
		for(j = 0; j < 2 * max + 1; j++)
			s.trace[i][j].k = j - max;
	}
	v[max + 1] = 0;
	for (d = 0; d < max; d++) {
		int xStart, xMid, yMid, xEnd, yEnd;
		s.d = d;
		for (k = -d; k <= d; k += 2) {
			int kPrev;
			int test = (k == -d || (k != d && v[max + k - 1] < v[max + k + 1]));
			if(test)
				kPrev = k + 1;
			else
				kPrev = k - 1;
			xStart = v[max + kPrev];
			xMid = test ? xStart : xStart + 1;
			yMid = xMid - k;
			xEnd = xMid;
			yEnd = yMid;
			if (iFlag)
				while (xEnd < n && yEnd < m && strcasecmp(str1[xEnd].lineText, str2[yEnd].lineText) == 0) {
					xEnd++;
					yEnd++;
				}
			else
				while (xEnd < n && yEnd < m && strcmp(str1[xEnd].lineText, str2[yEnd].lineText) == 0) {
					xEnd++;
					yEnd++;
				}
			v[max + k] = xEnd;
			for(i = 0; i < 2 * max + 1; i++)
				s.trace[d][i].v = v[i];
			if (xEnd >= n && yEnd >= m)
				return s;
		}
	}
	return s;
}

/* Finds out the edit script itself by backtracking */
editMap backtrack (lineInfo* str1, lineInfo* str2) {
	int x, y, max, i;
	solution s;
	x = size(str1);
	y = size(str2);
	max = x + y;
	s = editScript (str1, str2);
	editMap ed = NULL;
	for (i = s.d; x > 0 || y > 0; i--) {
		int kPrev, k = x - y;
		int xEnd = s.trace[i][max + k].v;
		int yEnd = x - k;
		int test = (k == -i || (k != i && s.trace[i][max + k - 1].v < s.trace[i][max + k + 1].v));
		if(test)
			kPrev = k + 1;
		else
			kPrev = k - 1;
		int xStart = s.trace[i][max + kPrev].v;
		int yStart = xStart > kPrev ? xStart - kPrev : 0;
		int xMid = test ? xStart : xStart + 1;
		int yMid = xMid - k;
		editRoute* new = (editRoute*) malloc (sizeof(editRoute));
		new->start.x = xStart;
		new->start.y = yStart;
		new->mid.x = xMid;
		new->mid.y = yMid;
		new->end.x = xEnd;
		new->end.y = yEnd;
		new->next = ed;
		new->prev = NULL;
		if(ed)
			new->next->prev = new;
		ed = new;
		x = xStart;
		y = yStart;
	}
	return ed;
}
