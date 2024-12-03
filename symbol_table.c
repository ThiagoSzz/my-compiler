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

// initialize global symbol table stack
void init_stack()
{
  stack = create_symbol_table_stack();

  SymbolTable *global_symbol_table = create_symbol_table();
  stack = add_table_to_stack(stack, global_symbol_table);
}

// create symbol table
SymbolTable *create_symbol_table()
{
  SymbolTable *symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
  if (!symbol_table)
  {
    printf("failed to create symbol table");
    return NULL;
  }

  symbol_table->size = 0;
  symbol_table->capacity = SYMBOL_TABLE_CAPACITY;
  symbol_table->items = (SymbolTableItem *)calloc(symbol_table->capacity, sizeof(SymbolTableItem));

  if (!symbol_table->items)
  {
    printf("failed to initialize symbol table items");
    free_symbol_table(symbol_table);
    return NULL;
  }

  return symbol_table;
}

// create stack
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

  return stack;
}

// add symbol table to stack top
Stack *add_table_to_stack(Stack *current_first_table, SymbolTable *symbol_table)
{
  if (!current_first_table)
  {
    printf("Adding table to null stack \n");
    return NULL;
  }

  if (!symbol_table)
  {
    printf("Adding null table to stack \n");
    return NULL;
  }

  Stack *new_first_table = create_symbol_table_stack();
  new_first_table->symbol_table = symbol_table;
  new_first_table->next = current_first_table;

  return new_first_table;
}

// add item value to stack
void add_value_to_table_on_stack(Stack *stack, SymbolTableItemValue value)
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

  SymbolTableItemValue item_value = find_item_value_by_key(table, value.lexical_value.label);

  if (item_value.nature != SYMBOL_NATURE_UNEXISTENT)
  {
    print_error("ERR_DECLARED", value.line_number, "failed to re-declare variable", value.lexical_value.label, item_value.line_number);
    exit(ERR_DECLARED);
  }

  add_item_to_list(table->items, table->capacity, value.lexical_value.label, &table->size, value);
}

// add item to symbol table linked list of items
void add_item_to_list(SymbolTableItem *items, int capacity, char *key, int *size, SymbolTableItemValue value)
{
  for (int i = 0; i < capacity; i++)
  {
    if (!items[i].key)
    {
      items[i].key = strdup(key);
      items[i].value = value;
      if (size)
      {
        (*size)++;
      }
      return;
    }
    else if (strcmp(items[i].key, key) == 0)
    {
      items[i].value = value;
      return;
    }
  }

  printf("failed to add item to list, no space left");
}

// create symbol table value with type
SymbolTableItemValue create_symbol_table_value_with_type(SymbolNatureEnum nature, LexicalValue lexical_value, DataType data_type)
{
  SymbolTableItemValue value;
  value.line_number = lexical_value.line_number;
  value.lexical_value = lexical_value;
  value.nature = nature;
  value.type = data_type;
  value.parameters = NULL;

  return value;
}

// create table on stack
Stack *create_table_on_stack(Stack *symbol_table_stack)
{
  SymbolTable *new_table = create_symbol_table();
  return add_table_to_stack(symbol_table_stack, new_table);
}

// create symbol table value with type and parameters
SymbolTableItemValue create_symbol_table_value_with_type_and_parameters(SymbolNatureEnum symbol_type, LexicalValue lexical_value, DataType data_type, Parameter *parameters)
{
  SymbolTableItemValue value = create_symbol_table_value_with_type(symbol_type, lexical_value, data_type);
  value.parameters = parameters;

  return value;
}

// add value to next table in stack
void add_value_to_next_table(Stack *stack, SymbolTableItemValue value)
{
  if (!stack)
  {
    printf("adding value to empty stack");
  }
  else
  {
    add_value_to_table_on_stack(stack->next, value);
  }
}

// create function parameter
Parameter *create_function_parameter(LexicalValue lexical_value, DataType data_type)
{
  Parameter *function_parameter = malloc(sizeof(Parameter));
  if (!function_parameter)
  {
    printf("failed to create function parameter");
  }

  function_parameter->next = NULL;
  function_parameter->type = data_type;
  function_parameter->lexical_value = lexical_value;

  return function_parameter;
}

// add function parameter
Parameter *add_function_parameter(Parameter *function_parameter, LexicalValue lexical_value, DataType data_type)
{
  Parameter *new_function_parameter = create_function_parameter(lexical_value, data_type);
  new_function_parameter->next = function_parameter;
  return new_function_parameter;
}

// validate and create function signature
void validate_create_function_signature(Stack *symbol_table_stack, LexicalValue lexical_value)
{
  while (symbol_table_stack)
  {
    SymbolTableItemValue value = find_item_value_by_key(symbol_table_stack->symbol_table, lexical_value.label);
    if (value.nature == SYMBOL_NATURE_FUNCTION)
    {
      print_error("ERR_DECLARED", lexical_value.line_number, "failed to re-declare function", lexical_value.label, value.line_number);
      exit(ERR_DECLARED);
    }
    symbol_table_stack = symbol_table_stack->next;
  }
}

// find variable value in stack by lexical value
SymbolTableItemValue find_variable_value_in_stack_by_lexical_value(Stack *symbol_table_stack, LexicalValue lexical_value)
{
  if (!symbol_table_stack)
  {
    print_error("ERR_UNDECLARED", lexical_value.line_number, "failed to use undeclared variable", lexical_value.label, -1);
    free_global_variables();
    exit(ERR_UNDECLARED);
  }

  SymbolTableItemValue value = find_item_value_by_key(symbol_table_stack->symbol_table, lexical_value.label);
  if (value.nature == SYMBOL_NATURE_UNEXISTENT)
  {
    return find_variable_value_in_stack_by_lexical_value(symbol_table_stack->next, lexical_value);
  }
  else
  {
    return value;
  }
}

// find function value in stack by lexical value
SymbolTableItemValue find_function_value_in_stack_by_lexical_value(Stack *symbol_table_stack, LexicalValue lexical_value)
{
  if (!symbol_table_stack)
  {
    print_error("ERR_UNDECLARED", lexical_value.line_number, "failed to use undeclared function", lexical_value.label, -1);
    free_global_variables();
    exit(ERR_UNDECLARED);
  }

  SymbolTableItemValue value = find_item_value_by_key(symbol_table_stack->symbol_table, lexical_value.label);
  if (value.nature == SYMBOL_NATURE_UNEXISTENT)
  {
    return find_function_value_in_stack_by_lexical_value(symbol_table_stack->next, lexical_value);
  }
  else
  {
    return value;
  }
}

// validate variable use
void validate_variable_use(SymbolTableItemValue symbol, LexicalValue lexical_value)
{
  if (symbol.nature != SYMBOL_NATURE_VARIABLE)
  {
    print_error("ERR_FUNCTION", lexical_value.line_number, "failed to use as a variable", symbol.lexical_value.label, -1);
    free_global_variables();
    if (symbol.nature == SYMBOL_NATURE_FUNCTION)
      exit(ERR_FUNCTION);
  }
}

// validate function call
void validate_function_call(SymbolTableItemValue symbol, LexicalValue lexical_value)
{
  if (symbol.nature != SYMBOL_NATURE_FUNCTION)
  {
    print_error("ERR_VARIABLE", lexical_value.line_number, "failed to use as a function call", symbol.lexical_value.label, -1);
    free_global_variables();
    if (symbol.nature == SYMBOL_NATURE_VARIABLE)
      exit(ERR_VARIABLE);
  }
}

// free global variables
void free_global_variables()
{
  free_ast(tree);
  free_symbol_table_stack(stack);
  yylex_destroy();
}

// free symbol table stack
void free_symbol_table_stack(Stack *symbol_table_stack)
{
  if (!symbol_table_stack)
    return;

  free_symbol_table_stack(symbol_table_stack->next);

  free_symbol_table(symbol_table_stack->symbol_table);

  free(symbol_table_stack);
  symbol_table_stack = NULL;
}

// pop stack
Stack *pop_stack(Stack *symbol_table_stack)
{
  if (!symbol_table_stack)
    return NULL;

  free_symbol_table(symbol_table_stack->symbol_table);

  Stack *next_item = symbol_table_stack->next;

  free(symbol_table_stack);
  symbol_table_stack = NULL;

  return next_item;
}

// free parameters
void free_parameters(Parameter *parameter)
{
  if (!parameter)
    return;

  free_parameters(parameter->next);

  free(parameter);
  parameter = NULL;
}

// free item value
void free_item_value(SymbolTableItemValue value)
{
  free_parameters(value.parameters);
  free_lexical_value(value.lexical_value);
}

// free item
void free_item(SymbolTableItem item)
{
  free_item_value(item.value);
  if (item.key)
  {
    free(item.key);
    item.key = NULL;
  }
}

// free table
void free_table(SymbolTable *table)
{
  for (size_t index = 0; index < table->capacity; index++)
  {
    free_item(table->items[index]);
  }
  free(table->items);
  table->items = NULL;
}

// free symbol table
void free_symbol_table(SymbolTable *table)
{
  if (!table)
    return;

  free_table(table);

  free(table);
  table = NULL;
}

// print stack
void print_stack(Stack *symbol_table_stack)
{
  if (!symbol_table_stack)
    return;
  if (!symbol_table_stack->symbol_table)
    return;

  SymbolTable *table = symbol_table_stack->symbol_table;

  printf("---->> TABLE <<----\n");
  for (size_t index = 0; index < table->capacity; index++)
  {
    SymbolTableItem item = table->items[index];
    if (item.key)
    {
      printf("[key=%s] [type=%s] [nature=%s]", item.key, get_type_string(item.value.type), get_nature_string(item.value.nature));
      Parameter *current_parameter = item.value.parameters;
      while (current_parameter)
      {
        printf("\n--> parameter %s [type=%s]", current_parameter->lexical_value.label, get_type_string(current_parameter->type));
        current_parameter = current_parameter->next;
      }
      printf("\n");
    }
  }
  printf("---->> END <<----\n\n");

  print_stack(symbol_table_stack->next);
}

// convert data type enum item to string
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

// convert nature enum item to string
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

// get new item value with unexistent nature
SymbolTableItemValue get_unexistent_value()
{
  SymbolTableItemValue value;
  value.nature = SYMBOL_NATURE_UNEXISTENT;

  return value;
}

// find item value by key in symbol table
SymbolTableItemValue find_item_value_by_key(SymbolTable *symbol_table, char *key)
{
  if (!symbol_table || !key)
    return get_unexistent_value();

  for (int i = 0; i < symbol_table->capacity; i++)
  {
    SymbolTableItem item = symbol_table->items[i];
    if (item.key && strcmp(item.key, key) == 0)
    {
      return item.value;
    }
  }

  return get_unexistent_value();
}

// print error message with pattern
void print_error(const char *error_type, int line_number, const char *message, const char *label, int previous_line_number)
{
  if (previous_line_number != -1)
  {
    printf("%s (line %d): %s \"%s\" that was declared before (line %d)\n", error_type, line_number, message, label, previous_line_number);
  }
  else
  {
    printf("%s (line %d): %s \"%s\"\n", error_type, line_number, message, label);
  }
}