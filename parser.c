//parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"

FILE *inputfp = NULL;

token_t tk;

int lineNum = 1;

static const char *tokenName[] ={
  [EOF_tk] = "EOF_tk", 
	[ERROR_tk] = "ERROR_tk", 
	[IDENTIFIER_tk] = "IDENTIFIER_tk", 
	[INTEGER_tk] = "INTEGER_tk", 
  [EQUAL_tk] = "EQUAL_tk", // =
	[LESS_tk] = "LESS_tk", // <
	[LESSEQ_tk] = "LESSEQ_tk", // <=
	[GREAT_tk] = "GREAT_tk", // >
	[GREATEQ_tk] = "GREATEQ_tk", // >=
	[NOTEQ_tk] = "NOTEQ_tk", // !=
	[EQEQ_tk] = "EQEQ_tk",  // == 
  [COLON_tk] = "COLON_tk", // :
	[PLUS_tk] = "PLUS_tk",  // +
	[MINUS_tk] = "MINUS_tk",  // -
	[ASTERISK_tk] = "ASTERISK_tk", // * 
	[SLASH_tk] = "SLASH_tk", // /
	[PCT_tk] = "PCT_tk", // %
	[PERIOD_tk] = "PERIOD_tk", // .
  [OPAR_tk] = "OPAR_tk", // (
	[CPAR_tk] = "CPAR_tk",  // )
	[COMMA_tk] = "COMMA_tk",  // ,
	[OBRACE_tk] = "OBRACE_tk", // {
	[CBRACE_tk] = "CBRACE_tk", // }
	[SCOLON_tk] = "SCOLON_tk", // ;
	[OBRACKET_tk] = "OBRACKET_tk", // [
	[CBRACKET_tk] = "CBRACKET_tk", // ]
	[BEGIN_tk] = "BEGIN_tk", 
	[END_tk] = "END_tk", 
	[ITER_tk] = "ITER_tk",  
	[VOID_tk] = "VOID_tk", 
	[VAR_tk] = "VAR_tk", 
	[RETURN_tk] = "RETURN_tk", 
	[READ_tk] = "READ_tk", 
	[PRINT_tk] = "OUTPUT_tk", 
	[PROGRAM_tk] = "PROGRAM_tk",
  [COND_tk] = "COND_tk",
  [THEN_tk] = "THEN_tk",
  [LET_tk] = "LET_tk"
 }; 
  
 //parser function
 node_t *parser(){
	node_t *node;
	tk = scanner(inputfp, &lineNum);
	node = program();
 
	if(tk.tkIdentifier != EOF_tk){
		printf("Error: Parse Failed. Expected EOF_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}		
	return node;
}

//-----------
//Program -> void <vars><block>
node_t *program(){

	int level = 0;
	node_t *node = createNode("<program>", level);
	if(tk.tkIdentifier == VOID_tk){									
		tk = scanner(inputfp, &lineNum);
		node->child1 = vars(level);
		node->child2 = block(level);
		return node;
	}
	else{
		printf("Error: Invalid Token, Expected VOID_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}
}

//-----------
node_t *block(int level){
	level++;
	node_t *node = createNode("<block>", level);
	if(tk.tkIdentifier == BEGIN_tk){											
		tk = scanner(inputfp, &lineNum);
		node->child1 = vars(level);
		node->child2 = stats(level);
		if (tk.tkIdentifier == END_tk){
			tk = scanner(inputfp, &lineNum);
			return node;
		}
		else{
			printf("Error: Invalid Token, Expected END_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Error: Invalid Token, Expected BEGIN_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}
}


//---------- <vars> -> empty | let Identifier = Integer <vars> 
node_t *vars(int level)
{
 
	level++;
	node_t *node = createNode("<vars>", level);
	if(tk.tkIdentifier == LET_tk)
  {											
		tk = scanner(inputfp, &lineNum);
   
		if(tk.tkIdentifier == IDENTIFIER_tk)
    {
			node->token = tk;
			tk = scanner(inputfp, &lineNum);
		  
        if(tk.tkIdentifier == EQUAL_tk)
        {
          node->token =tk;
          tk = scanner(inputfp, &lineNum);
            if(tk.tkIdentifier == INTEGER_tk)
            {
              node->token = tk;
              tk = scanner(inputfp, &lineNum);
              node->child1 = vars(level);
              return node;
            }
            else{
              printf("Error: Invalid Token, Expected INT_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
              exit(EXIT_FAILURE);
            }
        }
        else{
          printf("Error: Invalid Token, Expected EQUAL_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
          exit(EXIT_FAILURE);
        }
			return node;
		}
		else{
			printf("Error: Invalid Token, Expected IDENTIFIER_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		  exit(EXIT_FAILURE);
    }
	}
 
	return NULL;															
}

//------------------<expr> -> <A> / <expr> | <A> * <expr> | <A>
node_t *expr(int level)
{
  level++;
	node_t *node = createNode("<expr>", level);
  node->child1 = A(level);
  
  if(tk.tkIdentifier == SLASH_tk)
  {
    node->token =tk;
    tk = scanner(inputfp, &lineNum);
    node ->child2 = expr(level);
    return node;
  }
  else if(tk.tkIdentifier == ASTERISK_tk)
  {
    node->token = tk;
		tk = scanner(inputfp, &lineNum);
		node->child2 = expr(level);
		return node;
  }
  else{
    return node;
  }

}

//-------------<A> ->  <M> + <A> | <M> - <A> | <M>
node_t *A(int level)
{
  level++;
	node_t *node = createNode("<A>", level);
  node-> child1=M(level);
  
  if(tk.tkIdentifier == PLUS_tk)
  {
    node->token =tk;
    tk = scanner(inputfp, &lineNum);
    node->child2 = A(level);
    return node;
  }
  else if(tk.tkIdentifier == MINUS_tk) 
  {
    node->token = tk;
		tk = scanner(inputfp, &lineNum);
		node->child2 = A(level);
    return node;
  }
  else{
    return node;
  }
}

//---------------<M> -> - <M> |  <R>
node_t *M(int level)
{
  level++;
  node_t *node = createNode("<M>", level);
  
  if(tk.tkIdentifier == MINUS_tk)
  {
    node->child1 = M(level);
    node->token = tk;
    tk =scanner(inputfp, &lineNum);
    return node;
  }
  else
  {
    node->child1 = R(level);
    return node;
  }
}
//---------<R>-> ( <expr> ) | Identifier | Integer
node_t *R(int level)
{
  level++;
  node_t *node = createNode("<R>", level);
  if(tk.tkIdentifier == OPAR_tk)
  {
    tk = scanner(inputfp, &lineNum);
    node->child1 = expr(level);
  
    if(tk.tkIdentifier == CPAR_tk)
    {
      tk = scanner(inputfp, &lineNum);
      return node;
    }
    else{
    			printf("Error: Expected CPAR_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
          exit(EXIT_FAILURE);
    }
  }
  else if(tk.tkIdentifier == IDENTIFIER_tk)
  {
     node->token = tk;
     tk = scanner(inputfp, &lineNum);
     return node;   
  }
  else if(tk.tkIdentifier == INTEGER_tk)
  {
    node->token = tk;
    tk = scanner(inputfp, &lineNum);
    return node;
  }
  else{
    		printf("Error: Expected OPAR_tk, IDENTIFIER_tk or INTEGER_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
      exit(EXIT_FAILURE);
  }
  
}
//---------<stats> -> <stat> <mStat>
node_t *stats(int level)
{
  level++;		
	node_t *node = createNode("<stats>", level);
	node->child1 = stat(level);
	node->child2 = mStat(level);
	return node;
}

//-------<mStat> -> empty | <stat> <mStat>
node_t *mStat(int level)
{
  if(tk.tkIdentifier == READ_tk || tk.tkIdentifier == PRINT_tk || tk.tkIdentifier == COND_tk ||
	tk.tkIdentifier == BEGIN_tk || tk.tkIdentifier == ITER_tk || tk.tkIdentifier == IDENTIFIER_tk){
		level++;
		node_t *node = createNode("<mStat>", level);
		node->child1 = stat(level);
		node->child2 = mStat(level);
		return node;
	}
	else{
		return NULL;	
	}
}

//-------
node_t *stat(int level)
{
  level++;
	node_t *node = createNode("<stat>", level);
	if(tk.tkIdentifier == READ_tk){
		node->child1 = in(level);
		return node;
	}
	else if(tk.tkIdentifier == PRINT_tk){									
		node->child1 = out(level);
		return node;
	}
	else if(tk.tkIdentifier == BEGIN_tk){									
		node->child1 = block(level);
		return node;
	}
	else if(tk.tkIdentifier == COND_tk){								   
		node->child1 = if_func(level);
		return node;
	}
	else if(tk.tkIdentifier == ITER_tk){									
		node->child1 = loop(level);
		return node;
	}
	else if(tk.tkIdentifier == IDENTIFIER_tk){									    
		node->child1 = assign(level);
		return node;
	}
	else{
		printf("Parser Error: Expected READ_tk, PRINT_tk, BEGIN_tk, COND_tk, ITER_tk or IDENTIFIER_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}
}

//------<in> -> read ( Identifier ) :
node_t *in(int level)
{
  level++;
  node_t *node = createNode("<in>", level);
  if(tk.tkIdentifier == READ_tk)
  {
    tk = scanner(inputfp, &lineNum);
    if(tk.tkIdentifier == OPAR_tk )
    {
      tk = scanner(inputfp, &lineNum);
      if(tk.tkIdentifier == IDENTIFIER_tk)
      {
        node->token = tk;
				tk = scanner(inputfp, &lineNum);
				if(tk.tkIdentifier == CPAR_tk)
        {
					tk = scanner(inputfp, &lineNum);
							if(tk.tkIdentifier == COLON_tk)
           {
              tk = scanner(inputfp, &lineNum);
			      	return node;
           }
           else
           {
             	printf("Error: Invalid Token, Expected COLON_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
					exit(EXIT_FAILURE);
           }
				}
        else
        {
					printf("Error: Invalid Token, Expected CPAR_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
					exit(EXIT_FAILURE);
				}
      }
      else
      {
			printf("Error: Expected IDENTIFIER_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
			exit(EXIT_FAILURE);
		}
      
    }
    else{
			printf("Error: Invalid Token, Expected OPAR_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
			exit(EXIT_FAILURE);
		}
  }
  else{
		printf("Error: Invalid Token, Expected READ_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	} 
}

//------<out> -> PRINT ( Identifier ) :
node_t *out(int level)
{
  level++;
  node_t *node = createNode("<out>", level);
  if(tk.tkIdentifier == PRINT_tk)
  {
    tk = scanner(inputfp, &lineNum);
    if(tk.tkIdentifier == OPAR_tk )
    {
      tk = scanner(inputfp, &lineNum);
      node->child1 = expr(level);
       if(tk.tkIdentifier == CPAR_tk )
        {
        tk = scanner(inputfp, &lineNum);
        if(tk.tkIdentifier == COLON_tk )
        {
          tk = scanner(inputfp, &lineNum);
		    	return node;
        }
        else
        {
       	 printf("Error: Invalid Token, Expected COLON_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
					exit(EXIT_FAILURE);
        }
        }
         else
        {
       	 printf("Error: Invalid Token, Expected CPAR_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
					exit(EXIT_FAILURE);
        }
    }
     else
        {
       	 printf("Error: Invalid Token, Expected OPAR_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
					exit(EXIT_FAILURE);
        }
  } 
  return node;
}

//------------ <if> -> cond ( <expr> <RO> <expr> ) <stat> 
node_t *if_func(int level)
{
  if(tk.tkIdentifier == COND_tk){
		tk = scanner(inputfp, &lineNum);
		if(tk.tkIdentifier == OPAR_tk){
			level++;
			node_t *node = createNode("<if>", level);
			tk = scanner(inputfp, &lineNum);
			node->child1 = expr(level);
			node->child2 = RO(level);
			node->child3 = expr(level);
			if(tk.tkIdentifier == CPAR_tk){
				tk = scanner(inputfp, &lineNum);
				node->child4 = stat(level);
				return node;
			}
			else{
				printf("Parser Error: Expected OPAR_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("Parser Error: Expected CPAR_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser Error: Expected COND_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}

}
//----<loop> -> loop ( <expr> <RO> <expr> ) <stat> 
node_t *loop(int level)
{
  if(tk.tkIdentifier == ITER_tk)
  {
		level++;
		node_t *node = createNode("<loop>", level);
    tk = scanner(inputfp, &lineNum);
    if(tk.tkIdentifier == OPAR_tk)
      {
        tk = scanner(inputfp, &lineNum);
        node->child1 = expr(level);
	  		node->child2 = RO(level);
			  node->child3 = expr(level);
         if(tk.tkIdentifier == CPAR_tk)
         {
            tk = scanner(inputfp, &lineNum);
			    	node->child4 = stat(level);
			    	return node;
         }
         else{
			   	printf("Error: Expected CPAR_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
				  exit(EXIT_FAILURE);
			}
      }
      else{
			printf("Error: Expected OPAR_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
			exit(EXIT_FAILURE);
		}
   }
   else{
		printf("Error: Expected ITER_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}
}
//-----<assign>-> Identifier  = <expr> :
node_t *assign(int level)
{
    level++;
		node_t *node = createNode("<assign>", level);
   
       if(tk.tkIdentifier == IDENTIFIER_tk)
      {
        	node->token = tk;
			  tk = scanner(inputfp, &lineNum);
		  
         if(tk.tkIdentifier == EQUAL_tk)
           {
             
             tk = scanner(inputfp, &lineNum);
             	node->child1 = expr(level);
               if(tk.tkIdentifier == COLON_tk)
               {
                 tk=scanner(inputfp,&lineNum);
                 return node;
               }
               else{
                 printf("Error: Invalid Token, Expected COLON_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
				exit(EXIT_FAILURE);
               }
               
           }
           else{
			printf("Error: Invalid Token, Expected EQUAL_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
			exit(EXIT_FAILURE);
		}
           
      }
      else{
		printf("Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}
}

//-----<RO>->  < | <= | >  | >= | ==  | =
node_t *RO(int level)
{
  
level++;
	node_t *node = createNode("<RO>", level);
	if(tk.tkIdentifier == LESS_tk){					
		node->token = tk;
		tk = scanner(inputfp, &lineNum);
		return node;
	}
	else if(tk.tkIdentifier == LESSEQ_tk){			
			node->token2 = tk;
			tk = scanner(inputfp, &lineNum);
			return node;
	}
	else if(tk.tkIdentifier == GREAT_tk){			
		node->token = tk;
		tk = scanner(inputfp, &lineNum);
		return node;
	}
	else if(tk.tkIdentifier == GREATEQ_tk){			
			node->token2 = tk;
			tk = scanner(inputfp, &lineNum);
			return node;
	}
	else if(tk.tkIdentifier == EQEQ_tk){			
		node->token = tk;
		tk = scanner(inputfp, &lineNum);
		return node;
	}
	else if(tk.tkIdentifier == EQUAL_tk){			
		node->token = tk;
		tk = scanner(inputfp, &lineNum);
		return node;
	}
	else{
		printf("Parser Error: Expected LESS_tk, LESSEQ_tk, GREAT_tk, GREATEQ_tk, EQEQ_tk or EQUAL_tk -- %s Returned at Line %d\n", tokenName[tk.tkIdentifier], lineNum);
		exit(EXIT_FAILURE);
	}
}


//----------------------------------------------------------------------------------
//createNode function 
node_t *createNode(char *name, int level){
	node_t *newNode;
	newNode = malloc(sizeof(node_t));
 //not enough memory checking
	if(newNode == NULL){
		printf("ERROR: createNode function\n");
		exit(EXIT_FAILURE);
	}
	strcpy(newNode->name, name);
	newNode->level = level;
	newNode->child1 = NULL;
	newNode->child2 = NULL;
	newNode->child3 = NULL;
	newNode->child4 = NULL;
	return newNode;
}
