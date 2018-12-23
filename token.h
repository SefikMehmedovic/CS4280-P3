#ifndef TOKEN_H
#define TOKEN_H

// Enumeration of tokens  
typedef enum tokenId {
	// EOF, Error 
	EOF_tk, ERROR_tk,
	//  Identifiers and Integers 
	IDENTIFIER_tk, INTEGER_tk,
    //  Operators 
    EQUAL_tk, LESS_tk, LESSEQ_tk, GREAT_tk, GREATEQ_tk, 
	NOTEQ_tk, EQEQ_tk, COLON_tk, PLUS_tk, MINUS_tk, 
	ASTERISK_tk, SLASH_tk, PCT_tk, PERIOD_tk,
    // Delimiters 
    OPAR_tk, CPAR_tk, COMMA_tk, OBRACE_tk, CBRACE_tk, 
	SCOLON_tk, OBRACKET_tk, CBRACKET_tk,
	// Keywords 
	BEGIN_tk, END_tk, ITER_tk, VOID_tk, VAR_tk,
	RETURN_tk, READ_tk, PRINT_tk, PROGRAM_tk,
	COND_tk, THEN_tk, LET_tk
} token_id;

// Token definition 
typedef struct token{
	token_id tkIdentifier; 
	char *tkString;
	int lineNum;
} token_t;
						
#endif 