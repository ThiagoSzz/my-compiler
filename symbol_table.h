/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define SYMBOL_TABLE_CAPACITY 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_types.h"
#include "lexical_value.h"
#include "errors.h"

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
  int capacity;
} SymbolTable;

typedef struct Stack
{
  SymbolTable *symbol_table;
  struct Stack *next;
} Stack;

void init_stack();

SymbolTable *create_symbol_table();

Stack *create_symbol_table_stack();

Stack *add_table_to_stack(Stack *current_first_table, SymbolTable *symbol_table);

void add_value_to_table_on_stack(Stack *stack, SymbolTableItemValue value);

void add_item_to_list(SymbolTableItem *items, int capacity, char *key, int *size, SymbolTableItemValue value);

SymbolTableItemValue create_symbol_table_value_with_type(SymbolNatureEnum nature, LexicalValue lexical_value, DataType data_type);

Stack *create_table_on_stack(Stack *symbol_table_stack);

SymbolTableItemValue create_symbol_table_value_with_type_and_parameters(SymbolNatureEnum symbol_type, LexicalValue lexical_value, DataType data_type, Parameter *parameters);

void add_value_to_next_table(Stack *stack, SymbolTableItemValue value);

Parameter *create_function_parameter(LexicalValue lexical_value, DataType data_type);

Parameter *add_function_parameter(Parameter *function_parameter, LexicalValue lexical_value, DataType data_type);

void validate_create_function_signature(Stack *symbol_table_stack, LexicalValue lexical_value);

SymbolTableItemValue find_variable_value_in_stack_by_lexical_value(Stack *symbol_table_stack, LexicalValue lexical_value);

SymbolTableItemValue find_function_value_in_stack_by_lexical_value(Stack *symbol_table_stack, LexicalValue lexical_value);

void validate_variable_use(SymbolTableItemValue symbol, LexicalValue lexical_value);

void validate_function_call(SymbolTableItemValue symbol, LexicalValue lexical_value);

void free_global_variables();

void free_symbol_table_stack(Stack *symbol_table_stack);

Stack *pop_stack(Stack *symbol_table_stack);

void free_parameters(Parameter *parameter);

void free_item_value(SymbolTableItemValue value);

void free_item(SymbolTableItem item);

void free_table(SymbolTable *table);

void free_symbol_table(SymbolTable *table);

void print_stack(Stack *symbol_table_stack);

char *get_type_string(DataType type);

char *get_nature_string(SymbolNatureEnum nature);

SymbolTableItemValue get_unexistent_value();

SymbolTableItemValue find_item_value_by_key(SymbolTable *symbol_table, char *key);

void print_error(const char *error_type, int line_number, const char *message, const char *label, int previous_line_number);

#endif // SYMBOL_TABLE_H