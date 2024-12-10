#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"

extern int yyparse(void);
extern int yylex_destroy(void);

Node *tree;
Stack *stack;

int main(int argc, char **argv)
{
  int ret = yyparse();
  yylex_destroy();
  return ret;
}