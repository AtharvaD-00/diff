/* All the structures used for the program */

typedef struct lineInfo {
	char* lineText;
	int lineNumber;
	struct lineInfo* next;
} lineInfo;

typedef lineInfo* contents;

/* Function prototypes */
contents* lines (char* );
int set (char );
void copy (char* );
void npatch (contents* , char* );
void init (contents* );
int length (contents* );
void insertString (contents* , char* , int , int );
void deleteString (contents* , int );
void traverse (contents , char* );
char* string (contents* , int );
int max (contents* );
