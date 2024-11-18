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

Node *get_last_child(Node *parent);

void add_child(Node *parent, Node *child);

void free_ast(Node *node);

void print_node_labels(Node *node);

void print_node_connections(Node *node);

void exporta(Node *node);

#endif