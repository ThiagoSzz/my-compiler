/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#ifndef AST_H
#define AST_H

#include "lexical_value.h"
#include "common_types.h"
#include "symbol_table.h"

// AST data structure
typedef struct Node
{
  LexicalValue lexical_value;
  DataType data_type;
  struct Node *brother;
  struct Node *child;
  struct Node *parent;
} Node;

Node *create_node_with_value(LexicalValue lexical_value, SymbolTableItemValue value);

Node *create_node_with_type(LexicalValue lexical_value, DataType type);

Node *create_node_from_assignment(LexicalValue lexical_value, Node *left_child, Node *right_child);

Node *create_node_from_unary_operator(LexicalValue lexical_value, Node *child);

Node *create_node_from_binary_operator(LexicalValue lexical_value, Node *left_child, Node *right_child);

Node *create_function_call_node_with_value(LexicalValue lexical_value, SymbolTableItemValue value);

DataType type_inference(Node *node_one, Node *node_two);

int data_types_are_int_and_float(DataType type_one, DataType type_two);

Node *get_last_child(Node *parent);

void add_child(Node *parent, Node *child);

void free_ast(Node *node);

Node *initialize_node(LexicalValue lexical_value, DataType data_type);

void print_nodes(Node *node);

void print_edges(Node *node);

void export_ast(Node *node);

#endif // AST_H