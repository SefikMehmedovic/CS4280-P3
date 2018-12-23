#ifndef PARSER_H
#define PARSER_H

#include "node.h"

extern FILE *inputfp;

//parser function
node_t *parser();


node_t *program();
node_t *block(int level);
node_t *vars(int level);
node_t *expr(int level);
node_t *A(int level);
node_t *M(int level);
node_t *R(int level);
node_t *stats(int level);
node_t *mStat(int level);
node_t *stat(int level);
node_t *in(int level);
node_t *out(int level);
node_t *if_func(int level);
node_t *loop(int level);
node_t *assign(int level);
node_t *RO(int level);

//new node function
node_t *createNode(char *name, int level);

#endif