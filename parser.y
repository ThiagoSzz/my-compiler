/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

%{
  #include "common_types.h"
  #include <stdlib.h>

  int yylex(void);
  void yyerror (char const *mensagem);
  int get_line_number();

  DataType data_type = DATA_TYPE_UNDECLARED;
  struct Node* main_function_scope = NULL;
%}

%code requires {
  #include <stdio.h>

  #include "ast.h"
  #include "lexical_value.h"
  #include "symbol_table.h"
  #include "iloc_generator.h"

  extern Node *tree;
  extern Stack *stack;
}

%union
{
  DataType DataType;
  LexicalValue LexicalValue;
  struct Node *Node;
  struct Parameter *Parameter;
}

%define parse.error verbose

%token<LexicalValue> TK_PR_INT
%token<LexicalValue> TK_PR_FLOAT
%token<LexicalValue> TK_PR_IF
%token<LexicalValue> TK_PR_ELSE
%token<LexicalValue> TK_PR_WHILE
%token<LexicalValue> TK_PR_RETURN
%token<LexicalValue> TK_OC_LE
%token<LexicalValue> TK_OC_GE
%token<LexicalValue> TK_OC_EQ
%token<LexicalValue> TK_OC_NE
%token<LexicalValue> TK_OC_AND
%token<LexicalValue> TK_OC_OR
%token<LexicalValue> TK_IDENTIFICADOR
%token<LexicalValue> TK_LIT_INT
%token<LexicalValue> TK_LIT_FLOAT
%token<LexicalValue> '!' '*' '/' '%' '+' '-' '<' '>' '{' '}' '(' ')' '=' ',' ';'
%token TK_ERRO

%type<Node> program
%type<Node> start_program
%type<Node> end_program
%type<Node> list_of_functions
%type<DataType> type
%type<Node> literal
%type<Node> function
%type<Node> function_signature
%type<Parameter> list_of_parameters
%type<Node> function_body
%type<Node> block
%type<Node> start_of_block
%type<Node> end_of_block
%type<Node> commands
%type<Node> simple_command
%type<Node> end_of_command
%type<Node> variable_declaration
%type<Node> variable_list
%type<Node> variable_assignment
%type<Node> function_call
%type<Node> list_of_arguments
%type<Node> return_command
%type<Node> flow_control_statements
%type<Node> if_else_statement
%type<Node> end_if_else_statement
%type<Node> while_statement
%type<Node> end_while_statement
%type<Node> expression
%type<Node> precedence_8_operators
%type<Node> precedence_7_operators
%type<Node> precedence_6_operators
%type<Node> precedence_5_operators
%type<Node> precedence_4_operators
%type<Node> precedence_3_operators
%type<Node> precedence_2_operators
%type<Node> operand

%start program

%%

///////////////////////////////
/* -->>     Program     <<-- */
///////////////////////////////

program:
  start_program list_of_functions end_program
  {
    $$ = $2;
    tree = $$;
  }
  | 
  {
    $$ = NULL;
    tree = $$;
  }
  ;

start_program:
  {
    init_stack();
  }

end_program:
  {
    if (main_function_scope->operation_list != NULL)
    {
      generate_code(main_function_scope->operation_list);
      free_operation_list(main_function_scope->operation_list);
    }
    free_stack(stack);
  }

list_of_functions:
  function list_of_functions
  {
    $$ = $1;
    add_child($$, $2);

    if ($2) 
    {
      append_operation_list($$->operation_list, $2->operation_list);
    }
  }
  | function
  {
    $$ = $1;
  }
  ;

///////////////////////////////
/* -->>       Type      <<-- */
///////////////////////////////

type: 
  TK_PR_INT
  {
    data_type = DATA_TYPE_INT;
    $$ = data_type;
    
    free_lexical_value($1);
  }
  | TK_PR_FLOAT
  {
    data_type = DATA_TYPE_FLOAT;
    $$ = data_type;

    free_lexical_value($1);
  }
  ;

///////////////////////////////
/* -->>     Literal     <<-- */
///////////////////////////////

literal: 
  TK_LIT_INT
  {
    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_UNEXISTENT, $1, DATA_TYPE_INT);
    insert_value_into_stack(stack, value);

    $$ = create_node_from_value($1, value);

    OperationList* operation_list = create_operation_list();

    int c1 = atoi($1.label);
    int r1 = get_register();

    Operation operation = create_operation(OPERATION_LOADI, c1, -1, r1, -1);
    insert_operation_into_list(operation_list, operation);

    $$->output_register = r1;
    $$->operation_list = operation_list;
  }
  | TK_LIT_FLOAT
  {
    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_UNEXISTENT, $1, DATA_TYPE_FLOAT);
    insert_value_into_stack(stack, value);

    $$ = create_node_from_value($1, value);
  }
  ;

///////////////////////////////
/* -->>     Function    <<-- */
///////////////////////////////

function: function_signature function_body
  {
    $$ = $1;
    add_child($$, $2);

    if ($2)
    {
      $$->operation_list = $2->operation_list;
    }

    if (strncmp($1->lexical_value.label, "main", 4) == 0)
    {
      main_function_scope = $$;
    }
  }
  ;

function_signature: TK_IDENTIFICADOR '=' list_of_parameters '>' type
  {
    validate_function_declaration(stack->next, $1);
    stack = create_table_on_stack(stack);
    SymbolTableItemValue value = create_symbol_table_value_with_parameters(SYMBOL_NATURE_FUNCTION, $1, $5, $3);
    insert_value_into_next_table(stack, value);

    $$ = create_node_from_value($1, value);
    free_lexical_value($2);
    free_lexical_value($4);
  }
  ;

list_of_parameters:
  TK_IDENTIFICADOR '<''-' type
  {
    $$ = create_function_parameter($1, $4);
    free_lexical_value($2);
    free_lexical_value($3);

    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_VARIABLE, $1, $4);
    insert_value_into_stack(stack, value);
  }
  | TK_IDENTIFICADOR '<''-' type TK_OC_OR list_of_parameters
  {
    $$ = insert_new_parameter($6, $1, $4);
    free_lexical_value($2);
    free_lexical_value($3);
    free_lexical_value($5);

    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_VARIABLE, $1, $4);
    insert_value_into_stack(stack, value);
  }
  | 
  {
    $$ = NULL;
  }
  ;

function_body: block
  {
    $$ = $1;
  }
  ;

///////////////////////////////
/* -->>  Command Block  <<-- */
///////////////////////////////

block: 
  start_of_block commands end_of_block
  {
    $$ = $2;
  }
  | start_of_block end_of_block
  {
    $$ = NULL;
  }
  ;

start_of_block: '{'
  {
    SymbolTable *block_symbol_table = create_symbol_table();
    stack = insert_table_into_stack(stack, block_symbol_table);
  }
  ;

end_of_block: '}'
  {
    stack = pop_stack(stack);
  }
  ;

commands:
  simple_command commands
  {
    if($1)
    {
      $$ = $1;
      add_child($$, $2);

      if ($1->operation_list)
      {
        append_operation_list($1->operation_list, $2->operation_list);
      }
      else
      {
        $$->operation_list = $2->operation_list;
      }
    }
    else
    {
      $$ = $2;
    }
  }
  | simple_command
  {
    $$ = $1;
  }
  ;

///////////////////////////////
/* -->> Simple Commands <<-- */
///////////////////////////////

simple_command: 
  variable_declaration end_of_command
  {
    $$ = $1;
  }
  | variable_assignment end_of_command
  {
    $$ = $1;
  }
  | flow_control_statements end_of_command
  {
    $$ = $1;
  }
  | return_command end_of_command
  {
    $$ = $1;
  }
  | block end_of_command
  {
    $$ = $1;
  }
  | function_call end_of_command
  {
    $$ = $1;
  }
  ;

end_of_command: ';'
  {
    free_lexical_value($1);
  }
  ;

variable_declaration: type variable_list
  {
    $$ = $2;
  }
  ;

variable_list: 
  TK_IDENTIFICADOR 
  { 
    $$ = NULL;

    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_VARIABLE, $1, data_type);
    insert_value_into_stack(stack, value);
  }
  | TK_IDENTIFICADOR ',' variable_list
  { 
    $$ = $3;

    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_VARIABLE, $1, data_type);
    insert_value_into_stack(stack, value);

    free_lexical_value($2);
  }
  | TK_IDENTIFICADOR TK_OC_LE literal 
  {
    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_VARIABLE, $1, data_type);
    insert_value_into_stack(stack, value);

    $$ = create_node_from_type($2, data_type);
    add_child($$, create_node_from_value($1, value));
    add_child($$, $3);
  }
  | TK_IDENTIFICADOR TK_OC_LE literal ',' variable_list 
  { 
    SymbolTableItemValue value = create_symbol_table_value(SYMBOL_NATURE_VARIABLE, $1, data_type);
    insert_value_into_stack(stack, value);
    
    $$ = create_node_from_type($2, data_type);
    add_child($$, create_node_from_value($1, value));
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($4);
  }
  ;

variable_assignment: TK_IDENTIFICADOR '=' expression
  {
    SymbolTableItemValue value = find_variable_value_by_lexical_value(stack, $1);
    validate_variable_use(value, $1);

    Node* identifier = create_node_from_type($1, value.type);
    $$ = create_node_from_inferred_type($2, identifier, $3);
    add_child($$, identifier);
    add_child($$, $3);
    
    $$->operation_list = generate_store_identifier_code(value, $3);
  }
  ;

function_call: 
  TK_IDENTIFICADOR '(' list_of_arguments ')'
  {
    SymbolTableItemValue value = find_function_value_by_lexical_value(stack, $1);
    validate_function_use(value, $1);

    $$ = create_function_call_node_from_value($1, value);
    add_child($$, $3);
    free_lexical_value($2);
    free_lexical_value($4);
  }
  | TK_IDENTIFICADOR '(' ')'
  {
    SymbolTableItemValue value = find_function_value_by_lexical_value(stack, $1);
    validate_function_use(value, $1);

    $$ = create_function_call_node_from_value($1, value);
    free_lexical_value($2);
    free_lexical_value($3);
  }
  ;

list_of_arguments:
  expression ',' list_of_arguments
  {
    $$ = $1;
    add_child($$, $3);
    free_lexical_value($2);
  }
  | expression
  {
    $$ = $1;
  }
  ;

return_command: TK_PR_RETURN expression
  {
    $$ = create_node_from_child_type($1, $2);
    add_child($$, $2);
  }
  ;

flow_control_statements:
  if_else_statement
  {
    $$ = $1;
  }
  | while_statement
  {
    $$ = $1;
  }
  ;

if_else_statement: 
  TK_PR_IF '(' expression end_if_else_statement block
  {
    $$ = create_node_from_child_type($1, $3);
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($2);

    $$->operation_list = generate_if_statement_code($3, $5);
  }
  | TK_PR_IF '(' expression end_if_else_statement block TK_PR_ELSE block
  {
    $$ = create_node_from_child_type($1, $3);
    add_child($$, $3);
    add_child($$, $5);
    add_child($$, $7);
    free_lexical_value($2);
    free_lexical_value($6);

    $$->operation_list = generate_if_else_statement_code($3, $5, $7);
  }
  ;

end_if_else_statement: ')' 
  {
    $$ = NULL;
    free_lexical_value($1);

    stack = create_table_on_stack(stack);
  }
  ;

while_statement: TK_PR_WHILE '(' expression end_while_statement block
  {
    $$ = create_node_from_child_type($1, $3);
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($2);

    $$->operation_list = generate_while_statement_code($3, $5);
  }
  ;

end_while_statement: ')'
  {
    $$ = NULL;
    free_lexical_value($1);

    stack = create_table_on_stack(stack);
  }

///////////////////////////////
/* -->>   Expressions   <<-- */
///////////////////////////////

expression: precedence_8_operators
  {
    $$ = $1;
  }
  ;

precedence_8_operators:
  precedence_8_operators TK_OC_OR precedence_7_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_binary_expression_code(OPERATION_OR, $1, $3, &($$->output_register));
  }
  | precedence_7_operators
  {
    $$ = $1;
  }
  ;

precedence_7_operators:
  precedence_7_operators TK_OC_AND precedence_6_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_binary_expression_code(OPERATION_AND, $1, $3, &($$->output_register));
  }
  | precedence_6_operators
  {
    $$ = $1;
  }
  ;

precedence_6_operators:
  precedence_6_operators TK_OC_EQ precedence_5_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_comparison_expression_code(OPERATION_CMP_EQ, $1, $3, &($$->output_register));
  }
  | precedence_6_operators TK_OC_NE precedence_5_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_comparison_expression_code(OPERATION_CMP_NE, $1, $3, &($$->output_register));
  }
  | precedence_5_operators
  {
    $$ = $1;
  }
  ;

precedence_5_operators:
  precedence_5_operators '<' precedence_4_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
     $$->operation_list = generate_comparison_expression_code(OPERATION_CMP_LT, $1, $3, &($$->output_register));
  }
  | precedence_5_operators '>' precedence_4_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);

    $$->operation_list = generate_comparison_expression_code(OPERATION_CMP_GT, $1, $3, &($$->output_register));
  }
  | precedence_5_operators TK_OC_LE precedence_4_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);

    $$->operation_list = generate_comparison_expression_code(OPERATION_CMP_LE, $1, $3, &($$->output_register));
  }
  | precedence_5_operators TK_OC_GE precedence_4_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);

    $$->operation_list = generate_comparison_expression_code(OPERATION_CMP_GE, $1, $3, &($$->output_register));
  }
  | precedence_4_operators
  {
    $$ = $1;
  }
  ;

precedence_4_operators:
  precedence_4_operators '+' precedence_3_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_arithmetic_expression_code(OPERATION_ADD, $1, $3, &($$->output_register));
  }
  | precedence_4_operators '-' precedence_3_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_arithmetic_expression_code(OPERATION_SUB, $1, $3, &($$->output_register));
  }
  | precedence_3_operators
  {
    $$ = $1;
  }
  ;

precedence_3_operators:
  precedence_3_operators '*' precedence_2_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_arithmetic_expression_code(OPERATION_MULT, $1, $3, &($$->output_register));
  }
  | precedence_3_operators '/' precedence_2_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
    
    $$->operation_list = generate_arithmetic_expression_code(OPERATION_DIV, $1, $3, &($$->output_register));
  }
  | precedence_3_operators '%' precedence_2_operators
  {
    $$ = create_node_from_inferred_type($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_2_operators
  {
    $$ = $1;
  }
  ;

precedence_2_operators:
  '-' operand
  {
    $$ = create_node_from_child_type($1, $2);
    add_child($$, $2);
    
    $$->operation_list = generate_unary_expression_code(OPERATION_NEG, $2, &($$->output_register));
  }
  | '!' operand
  {
    $$ = create_node_from_child_type($1, $2);
    add_child($$, $2);
    
    $$->operation_list = generate_unary_expression_code(OPERATION_NOT, $2, &($$->output_register));
  }
  | operand
  {
    $$ = $1;
  }
  ;

operand:
  TK_IDENTIFICADOR
  {
    SymbolTableItemValue value = find_variable_value_by_lexical_value(stack, $1);
    validate_variable_use(value, $1);

    $$ = create_node_from_type($1, value.type);
    
    $$->operation_list = generate_load_identifier_code(value, &($$->output_register));
  }
  | literal
  {
    $$ = $1;
  }
  | function_call
  {
    $$ = $1;
  }
  | '(' expression ')'
  {
    $$ = $2;
    free_lexical_value($1);
    free_lexical_value($3);
  }
  ;

///////////////////////////////
/* -->>   End Program   <<-- */
///////////////////////////////

%%

void yyerror (char const *message) {
  printf("ERR_SYNTAX (line %d): %s\n", get_line_number(), message);
}