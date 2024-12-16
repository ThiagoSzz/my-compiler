/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#ifndef ILOC_GENERATOR_H
#define ILOC_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "common_types.h"

////////////////////////////////////////////////
/* -->>          Data Structures         <<-- */
////////////////////////////////////////////////

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

#endif // ILOC_GENERATOR_H