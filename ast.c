/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

// create node with lexical value and symbol table item value
Node *create_node_with_value(LexicalValue lexical_value, SymbolTableItemValue value)
{
  return initialize_node(lexical_value, value.type);
}

// create node with lexical value and data type
Node *create_node_with_type(LexicalValue lexical_value, DataType type)
{
  return initialize_node(lexical_value, type);
}

// create node from assignment operation
Node *create_node_from_assignment(LexicalValue lexical_value, Node *left_child, Node *right_child)
{
  return initialize_node(lexical_value, left_child->data_type);
}

// create node from unary operator
Node *create_node_from_unary_operator(LexicalValue lexical_value, Node *child)
{
  return initialize_node(lexical_value, child->data_type);
}

// create node from binary operator
Node *create_node_from_binary_operator(LexicalValue lexical_value, Node *left_child, Node *right_child)
{
  return initialize_node(lexical_value, type_inference(left_child, right_child));
}

// create function call node with lexical value and symbol table item value
Node *create_function_call_node_with_value(LexicalValue lexical_value, SymbolTableItemValue value)
{
  Node *function_call_node = create_node_with_value(lexical_value, value);

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

// infer data type from nodes
DataType type_inference(Node *node_one, Node *node_two)
{
  if (node_one->data_type == node_two->data_type)
    return node_one->data_type;

  if (data_types_are_int_and_float(node_one->data_type, node_two->data_type))
    return DATA_TYPE_FLOAT;

  return DATA_TYPE_UNDECLARED;
}

// check if the data types are int and float
int data_types_are_int_and_float(DataType type_one, DataType type_two)
{
  if (type_one == DATA_TYPE_INT && type_two == DATA_TYPE_FLOAT)
  {
    return 1;
  }
  if (type_one == DATA_TYPE_FLOAT && type_two == DATA_TYPE_INT)
  {
    return 1;
  }
  return 0;
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

// helper: initialize common fields of node
Node *initialize_node(LexicalValue lexical_value, DataType data_type)
{
  Node *node = (Node *)malloc(sizeof(Node));
  node->lexical_value = lexical_value;
  node->data_type = data_type;
  node->brother = NULL;
  node->child = NULL;
  node->parent = NULL;

  return node;
}

// helper: print node labels using recursive DFS
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

// helper: print node connections using recursive DFS
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

// helper: export AST
void export_ast(Node *node)
{
  if (!node)
    return;

  print_nodes(node);
  print_edges(node);
}