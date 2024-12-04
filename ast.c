/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

// initialize common fields of node from lexical value and type
Node *create_node(LexicalValue lexical_value, DataType data_type)
{
  Node *node = (Node *)malloc(sizeof(Node));
  node->lexical_value = lexical_value;
  node->data_type = data_type;
  node->brother = NULL;
  node->child = NULL;
  node->parent = NULL;

  return node;
}

// create node with lexical value and symbol table item value
Node *create_node_from_value(LexicalValue lexical_value, SymbolTableItemValue value)
{
  return create_node(lexical_value, value.type);
}

// create node with lexical value and data type
Node *create_node_from_type(LexicalValue lexical_value, DataType type)
{
  return create_node(lexical_value, type);
}

// create node from assignment operation
Node *create_node_from_inferred_type(LexicalValue lexical_value, Node *left_child, Node *right_child)
{
  return create_node(lexical_value, get_type_inference_rules(left_child, right_child));
}

// create node from unary operator
Node *create_node_from_child_type(LexicalValue lexical_value, Node *child)
{
  return create_node(lexical_value, child->data_type);
}

// create function call node with lexical value and symbol table item value
Node *create_function_call_node_from_value(LexicalValue lexical_value, SymbolTableItemValue value)
{
  Node *function_call_node = create_node_from_value(lexical_value, value);

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

// helper function to infer data type from nodes types
DataType get_type_inference_rules(Node *node_one, Node *node_two)
{
  // rules:
  //   (int, int) --> int
  //   (float, float) --> float
  //   (float, int) --> float
  //   (int, float) --> float

  if (node_one->data_type == node_two->data_type)
    return node_one->data_type;
  else
    return DATA_TYPE_FLOAT;

  return DATA_TYPE_UNDECLARED;
}

// helper function to get last child of parent node
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

// helper function to print node labels using recursive DFS
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

// helper function to print node connections using recursive DFS
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

// helper function to export AST
void export_ast(Node *node)
{
  if (!node)
    return;

  print_nodes(node);
  print_edges(node);
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