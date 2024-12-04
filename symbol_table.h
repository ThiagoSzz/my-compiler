/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define SYMBOL_TABLE_MAX_SIZE 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_types.h"
#include "lexical_value.h"
#include "errors.h"

////////////////////////////////////////////////
/* -->>          Data Structures         <<-- */
////////////////////////////////////////////////
typedef enum
{
  SYMBOL_NATURE_FUNCTION,
  SYMBOL_NATURE_VARIABLE,
  SYMBOL_NATURE_UNEXISTENT
} SymbolNatureEnum;

typedef enum
{
  SYMBOL_DATA_TYPE_INT,
  SYMBOL_DATA_TYPE_FLOAT,
  SYMBOL_DATA_TYPE_UNDECLARED
} SymbolDataTypeEnum;

typedef struct Parameter
{
  DataType type;
  LexicalValue lexical_value;
  struct Parameter *next;
} Parameter;

typedef struct SymbolTableItemValue
{
  int line_number;
  SymbolNatureEnum nature;
  DataType type;
  LexicalValue lexical_value;
  Parameter *parameters;
} SymbolTableItemValue;

typedef struct SymbolTableItem
{
  char *key;
  struct SymbolTableItemValue value;
} SymbolTableItem;

typedef struct SymbolTable
{
  struct SymbolTableItem *items;
  int size;
  int max_size;
} SymbolTable;

typedef struct Stack
{
  SymbolTable *symbol_table;
  struct Stack *next;
} Stack;

////////////////////////////////////////////////
/* -->>     Initialization Functions     <<-- */
////////////////////////////////////////////////

void init_stack();

SymbolTable *create_symbol_table();

Stack *create_symbol_table_stack();

Stack *create_table_on_stack(Stack *stack);

////////////////////////////////////////////////
/* -->>        Insertion Functions       <<-- */
////////////////////////////////////////////////

Stack *insert_table_into_stack(Stack *current_first_table, SymbolTable *symbol_table);

void insert_value_into_stack(Stack *stack, SymbolTableItemValue value);

void insert_new_symbol_table_item(SymbolTableItem *items, int max_size, char *key, int *size, SymbolTableItemValue value);

void insert_value_into_next_table(Stack *stack, SymbolTableItemValue value);

Parameter *insert_new_parameter(Parameter *function_parameter, LexicalValue lexical_value, DataType data_type);

////////////////////////////////////////////////
/* -->>        Creation Functions        <<-- */
////////////////////////////////////////////////

SymbolTableItemValue create_symbol_table_value(SymbolNatureEnum nature, LexicalValue lexical_value, DataType data_type);

SymbolTableItemValue create_symbol_table_value_with_parameters(SymbolNatureEnum nature, LexicalValue lexical_value, DataType data_type, Parameter *parameters);

Parameter *create_function_parameter(LexicalValue lexical_value, DataType data_type);

////////////////////////////////////////////////
/* -->>       Validation Functions       <<-- */
////////////////////////////////////////////////

void validate_variable_declaration(Stack *stack, LexicalValue lexical_value);

void validate_function_declaration(Stack *stack, LexicalValue lexical_value);

void validate_variable_use(SymbolTableItemValue value, LexicalValue lexical_value);

void validate_function_use(SymbolTableItemValue value, LexicalValue lexical_value);

////////////////////////////////////////////////
/* -->>          Find Functions          <<-- */
////////////////////////////////////////////////

SymbolTableItemValue find_variable_value_by_lexical_value(Stack *stack, LexicalValue lexical_value);

SymbolTableItemValue find_function_value_by_lexical_value(Stack *stack, LexicalValue lexical_value);

SymbolTableItemValue find_item_value_by_key(SymbolTable *symbol_table, char *key);

////////////////////////////////////////////////
/* -->>        Utility Functions         <<-- */
////////////////////////////////////////////////

char *get_type_string(DataType type);

char *get_nature_string(SymbolNatureEnum nature);

SymbolTableItemValue get_unexistent_value();

void print_error(const char *error_type, int line_number, const char *message, const char *label, int previous_line_number);

void print_stack(Stack *stack);

////////////////////////////////////////////////
/* -->>          Free Functions          <<-- */
////////////////////////////////////////////////

void free_ast_and_stack();

void free_stack(Stack *stack);

void free_parameters(Parameter *parameter);

void free_table_item_value(SymbolTableItemValue value);

void free_table_item(SymbolTableItem item);

void free_table_item_list(SymbolTable *table);

void free_symbol_table(SymbolTable *table);

Stack *pop_stack(Stack *stack);

#endif // SYMBOL_TABLE_H