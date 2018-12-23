//testTree.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "testTree.h"

void preOrderPrint(node_t *root){

  // if array is empty
	if (root == NULL){
		printf("Error: Array is Empty\n");
		return;
	}
	
	if(root->token.tkString != NULL && root->token2.tkString != NULL){
		printf("%*s%-9s\n", root->level*2," ", concat(concat(root->name, root->token.tkString), root->token2.tkString));
	}
	else if(root->token.tkString != NULL && root->token2.tkString == NULL){
		printf("%*s%-9s\n", root->level*2," ", concat(root->name, root->token.tkString));
	}
	else{
		printf("%*s%-9s\n", root->level*2," ", root->name);
	}

	if (root->child1 != NULL)
		preOrderPrint(root->child1);
	if (root->child2 != NULL)
		preOrderPrint(root->child2);
	if (root->child3 != NULL)
		preOrderPrint(root->child3);
	if (root->child4 != NULL)
		preOrderPrint(root->child4);
}

//concat strings function
char *concat(const char *s1, const char *s2){
  char *ns = malloc(snprintf(NULL, 0, "%s %s", s1, s2) + 1);
  if(ns == NULL){
	printf("Concat Failed\n");
	exit(EXIT_FAILURE);
  }
  sprintf(ns, "%s %s", s1, s2);
  return ns;
}