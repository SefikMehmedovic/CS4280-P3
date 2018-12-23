#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"

#define SPECIALCHARS 20
#define TOKENS 27
#define KEYWORDS 12


typedef struct charLookup{
	int input;
	int column;
}char_m;

typedef struct stateLookup{
	int state;
	token_id tkEnum;
}state_m;

// Scanner function 
token_t scanner(FILE *infile, int *lineNum);

//Token/Keyword mapping function 
token_t lookup(int state, char *searchString);

//Character mapping function
int matchFound(int currentChar);


#endif 
