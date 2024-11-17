#ifndef AST_H
#define AST_H

#include "lexical_value.h"

// AST data structure
typedef struct Node
{
  LexicalValue lexical_value;
  struct Node *brother;
  struct Node *child;
  struct Node *parent;
} Node;

Node *create_node(LexicalValue lexical_value);

void add_child(Node *parent, Node *child);

Node *get_last_child(Node *parent);

void print_ast(Node *node, int depth);

void free_ast(Node *node);

void exporta(void *arvore);

#endif