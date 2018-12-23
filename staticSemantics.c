//staticSemantics.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "staticSemantics.h"

//array of tokens as stack
token_t stack[MAXSTACKSIZE];

//number of vars 
static int stackVars = 0;

//Used to track starting point on stack 
static int scopeStart = 0;

//Loop counter var
int i = 0;

//Initialize all stack location strings to prevent comparison to uninitialized memory location(seg fault)
void stackInit(void){
	for(i = 0; i < MAXSTACKSIZE + 1; i++){
		stack[i].tkString = "";
	}
	return;
}

//Stack push 
void push(token_t stack_tk){

	//Warn stack overflow 
	if(stackVars == MAXSTACKSIZE){
		printf("Stack Is Full\n");
		exit(EXIT_FAILURE);
	}
	//Check for variable scope existence before insertion
	for(i = scopeStart; i < stackVars; i++){
		if(strcmp(stack[i].tkString, stack_tk.tkString) == 0){
			printf("%s Already Declared In This Scope On Line: %d\n", stack_tk.tkString, stack[i].lineNum);
			exit(EXIT_FAILURE);
		}
	}	
	//Push token to the stack 
	stack[stackVars] = stack_tk;
 
	//Increment number of vars on stack
	stackVars++;
}

//Pop scope vars when called 
void pop(int scopeStart){
	for(i = stackVars; i > scopeStart; i--){
		
		stackVars--;
		stack[i].tkString = NULL;
	}
}

//Make sure variable doesn't exist in the local scope stack before pushing
int find(char *var){
	//Search current scope for var string and return position 
	for(i = stackVars; i > scopeStart - 1; i--){
		if(strcmp(stack[i].tkString, var) == 0){
			return stackVars - 1 - i;
		}
	}
	
	return -1;
}

//Make sure variable exists in scope (or parent) before use 
int varExists(char *var){
	//Search current scope for var string and return position 
	for(i = stackVars - 1; i > - 1; i--){
		if(strcmp(stack[i].tkString, var) == 0){
			return stackVars - 1 - i;
		}
	}
	//Not found
	return -1;
}

//-------------------------------
void semanticCheck(node_t *node, int count){
	
	if(node == NULL){
		return;
	}
	//Process program  
	if(strcmp(node->name, "<program>") == 0){
		//has no vars
		int numVars = 0;
		//Process expected possible number of children if not null (<program> can have two child nodes) 
		if(node->child1 != NULL)
		semanticCheck(node->child1, numVars);
		if(node->child2 != NULL)
		semanticCheck(node->child2, numVars);
	}
 //-----block
   else if(strcmp(node->name, "<block>") == 0){
		int numVars = 0;
		scopeStart = stackVars;
		if(node->child1 != NULL)
		semanticCheck(node->child1, numVars);
		if(node->child2 != NULL)
		semanticCheck(node->child2, numVars);
		pop(scopeStart);
	}
 
	//Process vars 
	else if(strcmp(node->name, "<vars>") == 0){
		int found;
		scopeStart = stackVars;
		found = find(node->token.tkString);
		//If not found in scope or defined globally, push onto stack 
		if(found == -1 || found > count){
			push(node->token);
			count += 1;
		}
		//Error/exit 
		else if(found < count){
			printf("'%s' Is Already Defined Within This Scope On Line: %d\n", node->token.tkString, node->token.lineNum);
			exit(EXIT_FAILURE);
		}
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
	}
 
 //-------------------
 
 	else if(strcmp(node->name, "<mvars>") == 0){
		int found;
		if(count > 0){
			found = find(node->token.tkString);
			if(found == -1 || found > count){
				push(node->token);
				count += 1;
			}
			else if(found < count){
				printf("'%s' Is already defined within this Scope on Line: %d\n", node->token.tkString, node->token.lineNum);
				exit(EXIT_FAILURE);
			}
		}
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
	}
 
 //------------------
 
 
 //expr
	else if(strcmp(node->name, "<expr>") == 0){

		if(node->token.tkIdentifier == SLASH_tk){
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
			if(node->child2 != NULL)
			semanticCheck(node->child2, count);	
		}
		if(node->token.tkIdentifier == ASTERISK_tk){
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
			if(node->child2 != NULL)
			semanticCheck(node->child2, count);	
		}
		else{
			if(node->child1 != NULL)
			semanticCheck(node->child1, count);
		}
	}
// //A
//	else if(strcmp(node->name, "<A>") == 0){
//		if(node->token.tkIdentifier == PLUS_tk){
//			if(node->child1 != NULL)
//			semanticCheck(node->child1, count);
//			if(node->child2 != NULL)
//			semanticCheck(node->child2, count);	
//		}
//		if(node->token.tkIdentifier == MINUS_tk){
//			if(node->child1 != NULL)
//			semanticCheck(node->child1, count);
//			if(node->child2 != NULL)
//			semanticCheck(node->child2, count);	
//		}
//		else{
//			if(node->child1 != NULL)
//			semanticCheck(node->child1, count);
//		}
//	}
//  //-------------M	
//	else if(strcmp(node->name, "<M>") == 0){
//		if(node->token.tkIdentifier == MINUS_tk){
//			if(node->child1 != NULL)
//			semanticCheck(node->child1, count);
//		}
//		else{
//			if(node->child1 != NULL)
//			semanticCheck(node->child1, count);
//		}
//	}	
//  //R
//	else if(strcmp(node->name, "<R>") == 0){
// 
//		if(node->token.tkIdentifier == IDENTIFIER_tk){
//			int exists = varExists(node->token.tkString);
//			if(exists == -1){
//				printf("Variable1 '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNum);
//				exit(EXIT_FAILURE);
//			}
//		}
//		else{
//			if(node->child1 != NULL)
//			semanticCheck(node->child1, count);
//		}
//	}
//IN
//	else if(strcmp(node->name, "<in>") == 0){
// if(node->token.tkIdentifier == READ_tk){
//		int exists = varExists(node->token.tkString);
//		if(exists == -1){
//			printf("Variable2 '%s' Line: %d Not Declared in this Scope.\n", node->token.tkString, node->token.lineNum);
//			exit(EXIT_FAILURE);
//		}	
//	} 
// }
 //OUT
//	else if(strcmp(node->name, "<out>") == 0){
//		if(node->child1 != NULL)
//		semanticCheck(node->child1, count);
//	}
// //IF 
//	else if(strcmp(node->name, "<if>") == 0){
//		if(node->child1 != NULL)
//		semanticCheck(node->child1, count);
//		if(node->child3 != NULL)
//		semanticCheck(node->child3, count);
//		if(node->child4 != NULL)
//		semanticCheck(node->child4, count);
//	}	
// //LOOP
//	else if(strcmp(node->name, "<loop>") == 0){
//		if(node->child1 != NULL)
//		semanticCheck(node->child1, count);
//		if(node->child3 != NULL)
//		semanticCheck(node->child3, count);
//		if(node->child4 != NULL)
//		semanticCheck(node->child4, count);
//	}	
//	else if(strcmp(node->name, "<assign>") == 0){
//		int exists = varExists(node->token.tkString);
//		if(exists == -1){
//			printf("Variable3 '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNum);
//			exit(EXIT_FAILURE);
//		}
//		if(node->child1 != NULL)
//		semanticCheck(node->child1, count);
//	}
	//Catch all to process productions with only non-terminals
	else{
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
		if(node->child2 != NULL)
		semanticCheck(node->child2, count);
		if(node->child3 != NULL)
		semanticCheck(node->child3, count);
		if(node->child4 != NULL)
		semanticCheck(node->child4, count);
	}
	return;
}
