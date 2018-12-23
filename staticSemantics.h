//staticSemantics.h
#ifndef SEMANTICS_H
#define SEMANTICS_H

#define MAXSTACKSIZE 100
		
#include "token.h"
#include "node.h"

//Stack operations 
void push(token_t stack_tk);
void pop(int scopeStart);
void stackInit(void);

//Variable/stack search operations 
int find(char *var);
int varExists(char *var);

//Semantic check function
void semanticCheck(node_t *node, int count);


#endif 
