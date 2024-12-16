/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#ifndef ILOC_GENERATOR_H
#define ILOC_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

////////////////////////////////////////////////
/* -->>          Data Structures         <<-- */
////////////////////////////////////////////////

struct Node; // forward definition to prevent warning

typedef enum OperationsEnum
{
  OPERATION_UNDEFINED,
  OPERATION_NOP,
  OPERATION_MULT,
  OPERATION_DIV,
  OPERATION_NEG,
  OPERATION_NOT,
  OPERATION_SUB,
  OPERATION_ADD,
  OPERATION_AND,
  OPERATION_OR,
  OPERATION_CMP_GE,
  OPERATION_CMP_LE,
  OPERATION_CMP_GT,
  OPERATION_CMP_LT,
  OPERATION_CMP_NE,
  OPERATION_CMP_EQ,
  OPERATION_CBR,
  OPERATION_JUMPI,
  OPERATION_LOADI,
  OPERATION_LOADAI_GLOBAL,
  OPERATION_LOADAI_LOCAL,
  OPERATION_STOREAI_GLOBAL,
  OPERATION_STOREAI_LOCAL
} OperationsEnum;

typedef struct Operation
{
  OperationsEnum type;
  int label;
  int op1;
  int op2;
  int out1;
  int out2;
} Operation;

typedef struct OperationList
{
  Operation operation;
  struct OperationList *next_operation_list;
} OperationList;

////////////////////////////////////////////////
/* -->>     Initialization Functions     <<-- */
////////////////////////////////////////////////

int get_label();

int get_register();

Operation create_operation_from_type(OperationsEnum type);

Operation create_operation_from_type_and_label(OperationsEnum type, int label);

Operation create_operation(OperationsEnum type, int op1, int op2, int out1, int out2);

Operation create_operation_from_label(OperationsEnum type, int label, int op1, int op2, int out1, int out2);

////////////////////////////////////////////////
/* -->>        Generation Functions      <<-- */
////////////////////////////////////////////////

void map_operation_to_instruction(Operation operation);

void generate_code(OperationList *operation_list);

OperationList *generate_if_statement_code(struct Node *expression_node, struct Node *block_node);

OperationList *generate_if_else_statement_code(struct Node *expression_node, struct Node *if_block_node, struct Node *else_block_node);

OperationList *generate_while_statement_code(struct Node *expression_node, struct Node *block_node);

OperationList *generate_unary_expression_code(OperationsEnum operation_type, struct Node *node, int *output_register);

OperationList *generate_binary_expression_code(OperationsEnum operation_type, struct Node *left_node, struct Node *right_node, int *output_register);

OperationList *generate_comparison_expression_code(OperationsEnum operation_type, struct Node *left_node, struct Node *right_node, int *output_register);

OperationList *generate_arithmetic_expression_code(OperationsEnum operation_type, struct Node *left_node, struct Node *right_node, int *output_register);

OperationList *generate_load_identifier_code(SymbolTableItemValue value, int *output_register);

OperationList *generate_store_identifier_code(SymbolTableItemValue value, struct Node *expression_node);

////////////////////////////////////////////////
/* -->>        Creation Functions        <<-- */
////////////////////////////////////////////////

OperationList *create_operation_list();

OperationList *insert_new_operation_list(OperationList *operation_list);

////////////////////////////////////////////////
/* -->>        Insertion Functions       <<-- */
////////////////////////////////////////////////

void insert_operation_into_list(OperationList *operation_list, Operation operation);

void append_operation_list(OperationList *current_list, OperationList *new_list);

OperationList *merge_operation_list(OperationList *current_list, OperationList *new_list);

////////////////////////////////////////////////
/* -->>          Free Functions          <<-- */
////////////////////////////////////////////////

void free_operation_list(OperationList *operation_list);

#endif // ILOC_GENERATOR_H