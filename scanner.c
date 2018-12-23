#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "token.h"

//	FSA Table 
int FSATable[27][24] = {
	    //  WS      L     D     =     <     >     !     :     +     -     *     /     &     %     .    (      )     ,    {     }      ;    [      ]    EOF  
/*s0*/	{   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,  18,    19,   20,   21,   22,   -1},
/*s1*/	{1000,    1,    1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
/*s2*/	{1001, 1001,    2, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001},
/*s3*/	{1002, 1002, 1002,   23, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
/*s4*/	{1003, 1003, 1003,   24, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003},
/*s5*/	{1005, 1005, 1005,   25, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005},
/*s6*/	{  -2,   -2,   -2,   26,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2},
/*s7*/	{1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009},
/*s8*/	{1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010},
/*s9*/	{1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011},
/*s10*/	{1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012},
/*s11*/	{1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013},
/*s12*/	{1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014},
/*s13*/	{1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015},
/*s14*/	{1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016},
/*s15*/	{1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017},
/*s16*/	{1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018},
/*s17*/	{1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019},
/*s18*/	{1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020},
/*s19*/	{1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021},
/*s20*/	{1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022},
/*s21*/	{1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023},
/*s22*/	{1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
/*s23*/	{1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008},
/*s24*/	{1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004},
/*s25*/	{1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006},
/*s26*/	{1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007},
};

// Array holding charLokup structs for mapping special characters 

char_m specialChars[] = {
	{61, 3}, {60, 4}, {62, 5}, {58, 7}, {43, 8},		// =, <, >, :, +
	{45, 9}, {42, 10}, {47, 11}, {38, 12}, {37, 13},    // -, *, /, &, %
	{46, 14}, {40, 15}, {41, 16}, {44, 17}, {123, 18},	// ., (, ), ,, {
	{125, 19}, {59, 20}, {91, 21}, {93, 22}, {33, 6}	// }, ;, [, ]
};

// Final state to token struct mapping array
state_m finalStateLookup[] = {
	{-1, EOF_tk}, 
	{-2, ERROR_tk},
	{1000, IDENTIFIER_tk}, 
	{1001, INTEGER_tk},     
	{1002, EQUAL_tk},    
	{1003, LESS_tk},  
	{1004, LESSEQ_tk}, 
	{1005, GREAT_tk}, 
	{1006, GREATEQ_tk}, 
	{1007, NOTEQ_tk},    
	{1008, EQEQ_tk},  
	{1009, COLON_tk},  
	{1010, PLUS_tk}, 
	{1011, MINUS_tk},   
	{1012, ASTERISK_tk}, 
	{1013, SLASH_tk},  
	{1015, PCT_tk},
	{1016, PERIOD_tk},
	{1017, OPAR_tk}, 
	{1018, CPAR_tk}, 
	{1019, COMMA_tk}, 
	{1020, OBRACE_tk}, 
	{1021, CBRACE_tk},
	{1022, SCOLON_tk}, 
	{1023, OBRACKET_tk}, 
	{1024, CBRACKET_tk}
};

// Table used to lookup keywords 
token_t keywordLookupTable[] = {
{BEGIN_tk, "begin", 0}, 
{END_tk, "end", 0}, 
{ITER_tk, "iter", 0},
{VOID_tk, "void", 0}, 
{VAR_tk, "var", 0},
{RETURN_tk, "return", 0}, 
{READ_tk, "read", 0}, 
{PRINT_tk, "print", 0},
{PROGRAM_tk, "program", 0},
{COND_tk, "cond", 0},
{THEN_tk, "then", 0},
{LET_tk, "let", 0}  
};

// Scanner Function 
token_t scanner(FILE *infile, int *lineNum) {
	token_t returnTk;
	// FSA state implementation 
	int currentState = 0;
	int nextState = 0;

	// Return token values to defaults 
	returnTk.tkIdentifier = 999;
	returnTk.tkString = NULL;
	returnTk.lineNum = 999;

	int currentChar;
	int FSAColumn;
	char *currentWord = malloc(9);
	char *currentCharStr = malloc(2);

	// Continue to read from file 
	while (currentState < 500 && currentState > -1) {
		currentChar = fgetc(infile);

		// skip comments
		if (currentChar == '#') {
			while (!isspace(currentChar)) {
				currentChar = fgetc(infile);
			}
		}
			
		
		FSAColumn = matchFound(currentChar);
		sprintf(currentCharStr, "%c", currentChar);
		// Set nextState 
		nextState = FSATable[currentState][FSAColumn];

		
		if (FSAColumn == -2) {
			returnTk.tkIdentifier = ERROR_tk;
			returnTk.lineNum = *lineNum;
			strcpy(currentWord, currentCharStr);
			printf("\nScanner Error: Invalid Character: %c.\n", currentChar);
			returnTk.tkString = currentWord;
			return returnTk;
		}
		
		if (nextState > 999 || nextState == -1 || nextState == -2) {
			// EOF 
			if (nextState == -1) {
				returnTk.tkIdentifier = EOF_tk;
				returnTk.lineNum = *lineNum;
				returnTk.tkString = "EOF";
				return returnTk;
			}
			
			else if (nextState == 1004 || nextState == 1006 || nextState == 1007 || nextState == 1008) {
				sprintf(currentCharStr, "%c", currentChar);
				strcat(currentWord, currentCharStr);
				returnTk = lookup(nextState, currentWord);
				returnTk.lineNum = *lineNum;
				return returnTk;
			}
			// "Normal" final States 
			else if (nextState > 999) {
				returnTk = lookup(nextState, currentWord);
				returnTk.lineNum = *lineNum;
				ungetc(currentChar, infile);
				return returnTk;
			}
			// Error (-2) 
			else {
				ungetc(currentChar, infile);
				int temp = fgetc(infile);
				sprintf(currentCharStr, "%c", temp);
				strcat(currentWord, currentCharStr);
				printf("\nScanner Error: Line: %i. Invalid Character: %c. %s Not A Valid Construct.\n", *lineNum, currentChar, currentWord);
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNum = *lineNum;
				returnTk.tkString = currentWord;
				return returnTk;
			}
		}
	
		else{
			if(!isspace(currentChar)){
			strcat(currentWord, currentCharStr);
			}
			// 8 char limit
			if (strlen(currentWord) > 8){
				printf("Scanner Error: Identifier/Number Length Exception %s > 8\n", currentWord);
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNum = *lineNum;
				returnTk.tkString = "Length";
				return returnTk;
			}
			// Handle \n 
			if (currentChar == '\n') {
				(*lineNum)++;
			}
		
			currentState = nextState;
		}
	}
	
	returnTk.tkIdentifier = ERROR_tk;
	returnTk.lineNum = *lineNum;
	returnTk.tkString = "Scanner Error: Scanner Failure";
	return returnTk;
}


int matchFound(int currentChar) {     
	if (isalpha(currentChar)) {
		return 1;
	}
	else if (isdigit(currentChar)) { 
		return 2;
	}
	else if (isspace(currentChar)) {  
		return 0;
	}
	else if (currentChar == EOF) {   
		return 23;
	}
	else if(currentChar != EOF || !isspace(currentChar) || !isdigit(currentChar) || !isalpha(currentChar)){ 
		int l;
		for (l = 0; l < SPECIALCHARS; l++) {        
			if(specialChars[l].input == currentChar){
				return specialChars[l].column;
			}
		}
	}
	return -2;
}

// Function to find the CurrentWord from tokens 
token_t lookup(int state, char *searchString) {
	token_t returnTk;
	int i;
	for (i = 0; i < TOKENS; i++) {
		if (finalStateLookup[i].state == state) {
			returnTk.tkIdentifier = finalStateLookup[i].tkEnum;
			returnTk.tkString = searchString;
			returnTk.lineNum = 999; 
		}
	}
	
	if (state == 1000) {
		for (i = 0; i < KEYWORDS; i++) {
			if (strcmp(keywordLookupTable[i].tkString, searchString) == 0) {
				returnTk.tkIdentifier = keywordLookupTable[i].tkIdentifier;
				returnTk.tkString = keywordLookupTable[i].tkString;
				returnTk.lineNum = 999; 
			}
		}
	}
return returnTk;
}