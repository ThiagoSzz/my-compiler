/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#include "symbol_table.h"
#include "ast.h"

extern Node *tree;
extern Stack *stack;
extern int yylex_destroy(void);

// create and initialize global symbol table stack
void init_stack()
{
  stack = create_symbol_table_stack();

  SymbolTable *global_symbol_table = create_symbol_table();
  stack = insert_table_into_stack(stack, global_symbol_table);
  stack->is_global = 1;
  stack->last_position = 0;
}

// create and initialize symbol table
SymbolTable *create_symbol_table()
{
  SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
  if (!symbol_table)
  {
    printf("failed to create symbol table");
    return NULL;
  }

  symbol_table->size = 0;
  symbol_table->max_size = SYMBOL_TABLE_MAX_SIZE;
  symbol_table->items = (SymbolTableItem *)calloc(symbol_table->max_size, sizeof(SymbolTableItem));
  symbol_table->last_position = 0;

  if (!symbol_table->items)
  {
    printf("failed to initialize symbol table items");
    free_symbol_table(symbol_table);
    return NULL;
  }

  return symbol_table;
}

// create and initialize symbol table stack
Stack *create_symbol_table_stack()
{
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (!stack)
  {
    printf("failed to create stack");
    return NULL;
  }

  stack->symbol_table = NULL;
  stack->next = NULL;
  stack->is_global = 0;
  stack->last_position = 0;

  return stack;
}

// create symbol table and insert into symbol table stack
Stack *create_table_on_stack(Stack *stack)
{
  SymbolTable *new_table = create_symbol_table();
  return insert_table_into_stack(stack, new_table);
}

// insert symbol table into symbol table stack and add new table to the top
Stack *insert_table_into_stack(Stack *current_first_table, SymbolTable *symbol_table)
{
  if (!current_first_table || !symbol_table)
    return NULL;

  Stack *new_first_table = create_symbol_table_stack();
  new_first_table->symbol_table = symbol_table;
  new_first_table->next = current_first_table;

  if (stack->is_global)
  {
    new_first_table->last_position = 0;
  }
  else
  {
    symbol_table->last_position = stack->last_position;
    new_first_table->last_position = stack->last_position;
  }

  return new_first_table;
}

// validate and insert symbol table item value into symbol table stack
void insert_value_into_stack(Stack *stack, SymbolTableItemValue value)
{
  SymbolTable *table = stack->symbol_table;
  if (!stack || !table)
  {
    printf("failed to add item value to stack or table that is not initialized");
    return;
  }

  if (!value.lexical_value.label)
  {
    printf("failed to add item value to stack, label is not initialized");
    return;
  }

  value.is_global = stack->is_global;
  value.position = table->last_position;

  value.is_global = stack->is_global;
  if (value.nature == SYMBOL_NATURE_VARIABLE || value.nature == SYMBOL_NATURE_FUNCTION)
  {
    value.position = table->last_position;
    table->last_position += sizeof(int);
  }
  else
  {
    value.position = -1;
  }
  stack->last_position = table->last_position;

  validate_variable_declaration(stack, value.lexical_value);

  insert_new_symbol_table_item(table->items, table->max_size, value.lexical_value.label, &table->size, value);
}

// iterate structure and insert symbol table item into symbol table items linked list
void insert_new_symbol_table_item(SymbolTableItem *items, int max_size, char *key, int *size, SymbolTableItemValue value)
{
  for (int index = 0; index < max_size; index++)
  {
    if (!items[index].key)
    {
      items[index].key = strdup(key);
      items[index].value = value;
      if (size)
      {
        (*size)++;
      }
      return;
    }
    else if (strcmp(items[index].key, key) == 0)
    {
      items[index].value = value;
      return;
    }
  }

  printf("failed to add item to list, no space left");
}

// insert symbol table item value into next table in symbol table stack
void insert_value_into_next_table(Stack *stack, SymbolTableItemValue value)
{
  if (!stack)
    return;

  insert_value_into_stack(stack->next, value);
}

// insert new parameter into function parameter linked list
Parameter *insert_new_parameter(Parameter *function_parameter, LexicalValue lexical_value, DataType data_type)
{
  Parameter *new_function_parameter = create_function_parameter(lexical_value, data_type);
  new_function_parameter->next = function_parameter;
  return new_function_parameter;
}

// create symbol table item value
SymbolTableItemValue create_symbol_table_value(SymbolNatureEnum nature, LexicalValue lexical_value, DataType data_type)
{
  SymbolTableItemValue value;
  value.line_number = lexical_value.line_number;
  value.lexical_value = lexical_value;
  value.nature = nature;
  value.type = data_type;
  value.parameters = NULL;

  return value;
}

// create symbol table item value with parameters
SymbolTableItemValue create_symbol_table_value_with_parameters(SymbolNatureEnum nature, LexicalValue lexical_value, DataType data_type, Parameter *parameters)
{
  SymbolTableItemValue value = create_symbol_table_value(nature, lexical_value, data_type);
  value.parameters = parameters;

  return value;
}

// create and initialize function parameter
Parameter *create_function_parameter(LexicalValue lexical_value, DataType data_type)
{
  Parameter *function_parameter = (Parameter *)malloc(sizeof(Parameter));
  if (!function_parameter)
  {
    printf("failed to create function parameter");
  }

  function_parameter->next = NULL;
  function_parameter->type = data_type;
  function_parameter->lexical_value = lexical_value;

  return function_parameter;
}

// validate variable declaration and print error if variable is already declared
void validate_variable_declaration(Stack *stack, LexicalValue lexical_value)
{
  SymbolTableItemValue value = find_item_value_by_key(stack->symbol_table, lexical_value.label);
  if (value.nature != SYMBOL_NATURE_UNEXISTENT)
  {
    print_error("ERR_DECLARED", lexical_value.line_number, "failed to re-declare variable", lexical_value.label, value.line_number);
    exit(ERR_DECLARED);
  }
}

// validate function declaration and print error if function is already declared
void validate_function_declaration(Stack *stack, LexicalValue lexical_value)
{
  while (stack)
  {
    SymbolTableItemValue value = find_item_value_by_key(stack->symbol_table, lexical_value.label);
    if (value.nature == SYMBOL_NATURE_FUNCTION)
    {
      print_error("ERR_DECLARED", lexical_value.line_number, "failed to re-declare function", lexical_value.label, value.line_number);
      exit(ERR_DECLARED);
    }
    stack = stack->next;
  }
}

// validate variable use and print error if variable is not declared
void validate_variable_use(SymbolTableItemValue value, LexicalValue lexical_value)
{
  if (value.nature != SYMBOL_NATURE_VARIABLE)
  {
    print_error("ERR_FUNCTION", lexical_value.line_number, "failed to use as a variable", value.lexical_value.label, -1);
    free_ast_and_stack();
    if (value.nature == SYMBOL_NATURE_FUNCTION)
      exit(ERR_FUNCTION);
  }
}

// validate function call use and print error if function is not declared
void validate_function_use(SymbolTableItemValue value, LexicalValue lexical_value)
{
  if (value.nature != SYMBOL_NATURE_FUNCTION)
  {
    print_error("ERR_VARIABLE", lexical_value.line_number, "failed to use as a function call", value.lexical_value.label, -1);
    free_ast_and_stack();
    if (value.nature == SYMBOL_NATURE_VARIABLE)
      exit(ERR_VARIABLE);
  }
}

// find variable value in symbol table stack by lexical value
SymbolTableItemValue find_variable_value_by_lexical_value(Stack *stack, LexicalValue lexical_value)
{
  if (!stack)
  {
    print_error("ERR_UNDECLARED", lexical_value.line_number, "failed to use undeclared variable", lexical_value.label, -1);
    free_ast_and_stack();
    exit(ERR_UNDECLARED);
  }

  SymbolTableItemValue value = find_item_value_by_key(stack->symbol_table, lexical_value.label);
  if (value.nature == SYMBOL_NATURE_UNEXISTENT)
  {
    return find_variable_value_by_lexical_value(stack->next, lexical_value);
  }
  else
  {
    return value;
  }
}

// find function value in symbol table stack by lexical value
SymbolTableItemValue find_function_value_by_lexical_value(Stack *stack, LexicalValue lexical_value)
{
  if (!stack)
  {
    print_error("ERR_UNDECLARED", lexical_value.line_number, "failed to use undeclared function", lexical_value.label, -1);
    free_ast_and_stack();
    exit(ERR_UNDECLARED);
  }

  SymbolTableItemValue value = find_item_value_by_key(stack->symbol_table, lexical_value.label);
  if (value.nature == SYMBOL_NATURE_UNEXISTENT)
  {
    return find_function_value_by_lexical_value(stack->next, lexical_value);
  }
  else
  {
    return value;
  }
}

// find item value in symbol table by key and return unexistent value if not found
SymbolTableItemValue find_item_value_by_key(SymbolTable *symbol_table, char *key)
{
  if (!symbol_table || !key)
    return get_unexistent_value();

  for (int index = 0; index < symbol_table->max_size; index++)
  {
    SymbolTableItem item = symbol_table->items[index];
    if (item.key && strcmp(item.key, key) == 0)
    {
      return item.value;
    }
  }

  return get_unexistent_value();
}

// helper function to get string representation of data type
char *get_type_string(DataType type)
{
  switch (type)
  {
  case DATA_TYPE_INT:
    return "int";
  case DATA_TYPE_FLOAT:
    return "float";
  case DATA_TYPE_UNDECLARED:
    return "undeclared";
  default:
    return "unknown";
  }
}

// helper function to get string representation of symbol nature
char *get_nature_string(SymbolNatureEnum nature)
{
  switch (nature)
  {
  case SYMBOL_NATURE_VARIABLE:
    return "variable";
  case SYMBOL_NATURE_FUNCTION:
    return "function";
  default:
    return "unknown";
  }
}

// helper function to get unexistent value
SymbolTableItemValue get_unexistent_value()
{
  SymbolTableItemValue value;
  value.nature = SYMBOL_NATURE_UNEXISTENT;

  return value;
}

// helper function to print error message following the pattern
void print_error(const char *error_type, int line_number, const char *message, const char *label, int previous_line_number)
{
  if (previous_line_number != -1)
    printf("%s (line %d): %s \"%s\" that was declared before (line %d)", error_type, line_number, message, label, previous_line_number);
  else
    printf("%s (line %d): %s \"%s\"", error_type, line_number, message, label);
}

// helper function to print symbol table stack, used for debugging
void print_stack(Stack *stack)
{
  if (!stack || !stack->symbol_table)
    return;

  SymbolTable *table = stack->symbol_table;

  printf("---->> TABLE [%p] <<----\n", (void *)table);
  for (int index = 0; index < table->max_size; index++)
  {
    SymbolTableItem item = table->items[index];
    if (item.key)
    {
      printf("[%d] name=\"%s\", type=%s, nature=%s", index, item.key, get_type_string(item.value.type), get_nature_string(item.value.nature));
      Parameter *current_parameter = item.value.parameters;
      while (current_parameter)
      {
        printf("\n    [parameter] name=\"%s\", type=%s", current_parameter->lexical_value.label, get_type_string(current_parameter->type));
        current_parameter = current_parameter->next;
      }
      printf("\n");
    }
  }
  printf("---->> END <<----\n\n");

  print_stack(stack->next);
}

// free AST and symbol table stack
void free_ast_and_stack()
{
  free_ast(tree);
  free_stack(stack);
  yylex_destroy();
}

// free symbol table stack
void free_stack(Stack *stack)
{
  if (!stack)
    return;

  free_stack(stack->next);

  free_symbol_table(stack->symbol_table);

  free(stack);
  stack = NULL;
}

// free function parameter linked list
void free_parameters(Parameter *parameter)
{
  if (!parameter)
    return;

  free_parameters(parameter->next);

  free(parameter);
  parameter = NULL;
}

// free symbol table item value
void free_table_item_value(SymbolTableItemValue value)
{
  free_parameters(value.parameters);
  free_lexical_value(value.lexical_value);
}

// free symbol table item
void free_table_item(SymbolTableItem item)
{
  free_table_item_value(item.value);
  if (item.key)
  {
    free(item.key);
    item.key = NULL;
  }
}

// free symbol table item linked list
void free_table_item_list(SymbolTable *table)
{
  for (int index = 0; index < table->max_size; index++)
  {
    free_table_item(table->items[index]);
  }
  free(table->items);
  table->items = NULL;
}

// free symbol table
void free_symbol_table(SymbolTable *table)
{
  if (!table)
    return;

  free_table_item_list(table);

  free(table);
  table = NULL;
}

// free symbol table on top of stack
Stack *pop_stack(Stack *stack)
{
  if (!stack)
    return NULL;

  free_symbol_table(stack->symbol_table);

  Stack *next_item = stack->next;

  free(stack);
  stack = NULL;

  return next_item;
}
