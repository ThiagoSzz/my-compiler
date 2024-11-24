/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

// create AST node
Node *create_node(LexicalValue lexical_value)
{
  Node *node = (Node *)malloc(sizeof(Node));

  node->lexical_value = lexical_value;
  node->brother = NULL;
  node->child = NULL;
  node->parent = NULL;

  return node;
}

// create AST node for function call rules
Node *create_function_call_node(LexicalValue lexical_value)
{
  Node *function_call_node = create_node(lexical_value);

  // add "call " prefix to label
  char *prefix = "call ";
  char *label = malloc(strlen(function_call_node->lexical_value.label) + strlen(prefix) + 1);
  strcpy(label, prefix);
  strcat(label, function_call_node->lexical_value.label);

  // free old label
  free(function_call_node->lexical_value.label);

  // set new label
  function_call_node->lexical_value.label = label;

  return function_call_node;
}

// get last child of parent node
Node *get_last_child(Node *parent)
{
  Node *temp_last_child = NULL;
  Node *last_child = parent->child;

  // iterate through children to get last child
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

// print node labels using recursive DFS
void print_nodes(Node *node)
{
  printf("%p [label=\"%s\"];\n", node, node->lexical_value.label);
  if (node->child)
  {
    print_nodes(node->child);
  }
  if (node->brother)
  {
    print_nodes(node->brother);
  }
}

// print node connections using recursive DFS
void print_edges(Node *node)
{
  if (node->parent)
  {
    printf("%p, %p \n", node->parent, node);
  }
  if (node->child)
  {
    print_edges(node->child);
  }
  if (node->brother)
  {
    print_edges(node->brother);
  }
}

// print AST
void export_ast(Node *node)
{
  if (!node)
    return;

  print_nodes(node);
  print_edges(node);
}