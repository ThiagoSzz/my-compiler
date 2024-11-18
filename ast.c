#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

// create AST node
Node *create_node(LexicalValue lexical_value)
{
  Node *node = malloc(sizeof(Node));

  node->lexical_value = lexical_value;
  node->brother = NULL;
  node->child = NULL;
  node->parent = NULL;

  return node;
}

// get last child of parent node
Node *get_last_child(Node *parent)
{
  Node *temp_last_child = NULL;
  Node *last_child = parent->child;

  while (last_child)
  {
    temp_last_child = last_child;
    last_child = last_child->brother;
  }

  return temp_last_child;
}

// add child to parent node
void add_child(Node *parent, Node *child)
{
  if (!child)
    return;

  if (!parent)
  {
    free_ast(child);
    return;
  }

  Node *last_child = get_last_child(parent);
  if (last_child)
  {
    last_child->brother = child;
  }
  else
  {
    parent->child = child;
  }
  child->parent = parent;
}

// free AST
void free_ast(Node *node)
{
  if (!node)
    return;

  free_lexical_value(node->lexical_value);

  Node *child = node->child;
  free_ast(child);

  Node *brother = node->brother;
  free_ast(brother);

  free(node);
}

// print node labels
void print_node_labels(Node *node)
{
  printf("%p [label=\"%s\"];\n", node, node->lexical_value.label);
  if (node->child)
  {
    print_node_labels(node->child);
  }
  if (node->brother)
  {
    print_node_labels(node->brother);
  }
}

// print node connections
void print_node_connections(Node *node)
{
  if (node->parent)
  {
    printf("%p, %p \n", node->parent, node);
  }
  if (node->child)
  {
    print_node_connections(node->child);
  }
  if (node->brother)
  {
    print_node_connections(node->brother);
  }
}

// print AST
void exporta(Node *node)
{
  if (!node)
    return;

  print_node_labels(node);
  print_node_connections(node);
}