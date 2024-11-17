#include <stdio.h>
#include "ast.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta(void *arvore);
void free_ast(Node *node);

int main(int argc, char **argv)
{
  int ret = yyparse();
  exporta(arvore);
  free_ast(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}