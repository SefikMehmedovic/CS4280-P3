//main.c
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "node.h"
#include "testTree.h"
#include "staticSemantics.h"

int main(int argc, char* argv[]){
	// File pointer
//	FILE *inputfp = NULL;
	
	static const char *EXTENSION = ".fs18";
  	node_t *tree;
   stackInit();
    	// error if more than 2 arguments
	if (argc > 2){
		printf("Error: Exceeded Argument limit.\n");
		exit(EXIT_FAILURE);
	}
   
   //run from input if argc == 1 else run from file
	 if(argc == 1){
		// input file pointer 
		inputfp = stdin;
	
   // if stdin open fails
		if (inputfp == NULL){
			fprintf(stderr, "Unable To Start stdin: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		}

		// Prompt user for input
		if (isatty(0)){
			printf("\nFile Not Found. \nEnter Input: ");
		}
   
		if(!feof(inputfp)){
			tree = parser();
		}
   
   //Check Semantics 
    printf("Semantic Check Started\n");
		semanticCheck(tree, 0);
 	  printf("Semantic Check Complete\n");
   // print tree and close file
   
   //------------------------------------// un comment to run 
 		preOrderPrint(tree);
		fclose(inputfp);
	}
	else {
		
		char *fileName = malloc(sizeof(argv[1]) + sizeof(EXTENSION));
		strncat(fileName, argv[1], strlen(argv[1]));
		strncat(fileName, EXTENSION, strlen(EXTENSION));
		inputfp = fopen(fileName, "r");

		//  input file failure 
		if (inputfp == NULL){
			fprintf(stderr, "Unable To Open '%s': %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
		}

		// read token 
		if(!feof(inputfp)){
				tree = parser();
		}
   //----------
    //Check Semantics 
    printf("Semantic Check Started\n");
		semanticCheck(tree, 0);
 	  printf("Semantic Check Complete\n");
    //prints tree and closes file
  preOrderPrint(tree);
		fclose(inputfp);
	}
 //------------
 

	return 0; //main return
} //main
