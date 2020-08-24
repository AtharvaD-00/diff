/* All the structures used for the program */

typedef struct data {
	int** matrix;
	int length;
} data;

typedef struct lineInfo {
	char* lineText;
	int lineNumber;
} lineInfo;

typedef struct point {
	int v, k;
} point;

typedef struct solution {
	point** trace;
	int d;
} solution;

typedef struct editPoint {
	int x, y;
} editPoint;

typedef struct editRoute {
	editPoint start, mid, end;
	struct editRoute *next, *prev;
} editRoute;

typedef editRoute* editMap;


/* Function Prototypes */
int max (int , int );
data createMatrix (lineInfo* , lineInfo* , int , int );
lineInfo* subsequence (lineInfo* , lineInfo* );
lineInfo* construct (data , lineInfo* , lineInfo* , int , int );
lineInfo* subsequence (lineInfo* , lineInfo* );
lineInfo* lines (char* );
lineInfo* clone (lineInfo* );
void destroylines (lineInfo* );
int size (lineInfo* );
int subsize (lineInfo* );
int fcheck (char* );
int dcheck (char* );
solution editScript (lineInfo* , lineInfo* );
editMap backtrack (lineInfo* , lineInfo* );
void createDiff (editMap , lineInfo* , lineInfo* );
int set (char );
void removeSpaces (char* );
void noSpace (lineInfo* );
void removeExtraSpaces (char* );
void noExtraSpace (lineInfo* );
char* replaceTabs (char* );
void tabsToSpaces (lineInfo* );
int isBinary (char* );
void printFileProperties (char* );
lineInfo* listFiles (char* , int );
void files (char* , char* , int , char*[] );
void onedir (char* , char* , int );
