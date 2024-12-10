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

////////////////////////////////////////////////
/* -->>        Creation Functions        <<-- */
////////////////////////////////////////////////

Node *create_node(LexicalValue lexical_value, DataType data_type);

Node *create_node_from_value(LexicalValue lexical_value, SymbolTableItemValue value);

Node *create_node_from_type(LexicalValue lexical_value, DataType type);

Node *create_node_from_inferred_type(LexicalValue lexical_value, Node *left_child, Node *right_child);

Node *create_node_from_child_type(LexicalValue lexical_value, Node *child);

Node *create_function_call_node_from_value(LexicalValue lexical_value, SymbolTableItemValue value);

////////////////////////////////////////////////
/* -->>        Insertion Functions       <<-- */
////////////////////////////////////////////////

void add_child(Node *parent, Node *child);

////////////////////////////////////////////////
/* -->>        Utility Functions         <<-- */
////////////////////////////////////////////////

DataType get_type_inference_rules(Node *node_one, Node *node_two);

Node *get_last_child(Node *parent);

void print_nodes(Node *node);

void print_edges(Node *node);

void export_ast(Node *node);

////////////////////////////////////////////////
/* -->>          Free Functions          <<-- */
////////////////////////////////////////////////

void free_ast(Node *node);

#endif // AST_H