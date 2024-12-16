#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"
#include "iloc_generator.h"

extern int yyparse(void);
extern int yylex_destroy(void);

Node *tree;
Stack *stack;

int main(int argc, char **argv)
{
  int ret = yyparse();
  return ret;
}