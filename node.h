#ifndef NODE_H
#define NODE_H

#include "token.h"
#define LENGTH 10 //max length of name					

typedef struct node{

	char name[LENGTH];
	int level;

	token_t token;
	token_t token2;
	
	struct node *child1;
	struct node *child2;
	struct node *child3;
	struct node *child4;
} node_t;


#endif 