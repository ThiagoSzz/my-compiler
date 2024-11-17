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

// add child to parent node
void add_child(Node *parent, Node *child)
{
  if (!child)
    return;

  if (!parent)
    free_ast(child);
  return;

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

void print_node(Node *node)
{
  if (node == NULL)
    return;
  printf("%p [label=\"", node);
  printf("%s", node->lexical_value.label);
  printf("\"];\n");

  Node *node_child;
  node_child = node->child;
  while (node_child != NULL)
  {
    print_node(node_child);
    node_child = node_child->brother;
  }

  return;
}

void print_edge(Node *node)
{
  if (node == NULL)
    return;

  Node *node_child;
  node_child = node->child;
  while (node_child != NULL)
  {
    printf("%p, %p\n", node, node_child);
    print_edge(node_child);
    node_child = node_child->brother;
  }

  return;
}

void exporta(void *arvore)
{
  Node *node;
  node = (Node *)arvore;
  print_node(node);
  print_edge(node);
  return;
}