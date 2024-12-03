/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

%{
  #include "common_types.h"

  int yylex(void);
  void yyerror (char const *mensagem);
  int get_line_number();

  DataType data_type = DATA_TYPE_UNDECLARED;
%}

%code requires {
  #include <stdio.h>

  #include "ast.h"
  #include "lexical_value.h"
  #include "symbol_table.h"

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
  list_of_functions 
  {
    $$ = $1;
    tree = $$;
  }
  | 
  {
    $$ = NULL;
    tree = $$;
  }
  ;

list_of_functions:
  function list_of_functions
  {
    $$ = $1;
    add_child($$, $2);
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
    SymbolTableItemValue value = create_symbol_table_value_with_type(SYMBOL_NATURE_UNEXISTENT, $1, DATA_TYPE_INT);
    add_value_to_table_on_stack(stack, value);

    $$ = create_node_with_value($1, value);
  }
  | TK_LIT_FLOAT
  {
    SymbolTableItemValue value = create_symbol_table_value_with_type(SYMBOL_NATURE_UNEXISTENT, $1, DATA_TYPE_FLOAT);
    add_value_to_table_on_stack(stack, value);
    
    $$ = create_node_with_value($1, value);
  }
  ;

///////////////////////////////
/* -->>     Function    <<-- */
///////////////////////////////

function: function_signature function_body
  {
    $$ = $1;
    add_child($$, $2);
  }
  ;

function_signature: TK_IDENTIFICADOR '=' list_of_parameters '>' type
  {
    validate_create_function_signature(stack->next, $1);

    stack = create_table_on_stack(stack);

    SymbolTableItemValue symbol = create_symbol_table_value_with_type_and_parameters(SYMBOL_NATURE_FUNCTION, $1, $5, $3);
    add_value_to_next_table(stack, symbol);

    $$ = create_node_with_value($1, symbol);

    free_lexical_value($2);
    free_lexical_value($4);
  }
  ;

list_of_parameters:
  TK_IDENTIFICADOR '<''-' type
  {
    $$ = create_function_parameter($1, $4);

    SymbolTableItemValue symbol = create_symbol_table_value_with_type(SYMBOL_NATURE_VARIABLE, $1, $4);
    add_value_to_table_on_stack(stack, symbol);

    free_lexical_value($2);
    free_lexical_value($3);
  }
  | TK_IDENTIFICADOR '<''-' type TK_OC_OR list_of_parameters
  {
    $$ = add_function_parameter($6, $1, $4);

    SymbolTableItemValue symbol = create_symbol_table_value_with_type(SYMBOL_NATURE_VARIABLE, $1, $4);
    add_value_to_table_on_stack(stack, symbol);

    free_lexical_value($2);
    free_lexical_value($3);
    free_lexical_value($5);
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
    stack = add_table_to_stack(stack, block_symbol_table);
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

    SymbolTableItemValue symbol = create_symbol_table_value_with_type(SYMBOL_NATURE_VARIABLE, $1, data_type);
    add_value_to_table_on_stack(stack, symbol);
  }
  | TK_IDENTIFICADOR ',' variable_list
  { 
    $$ = $3;

    SymbolTableItemValue symbol = create_symbol_table_value_with_type(SYMBOL_NATURE_VARIABLE, $1, data_type);
    add_value_to_table_on_stack(stack, symbol);

    free_lexical_value($2);
  }
  | TK_IDENTIFICADOR TK_OC_LE literal 
  {
    SymbolTableItemValue symbol = create_symbol_table_value_with_type(SYMBOL_NATURE_VARIABLE, $1, data_type);
    add_value_to_table_on_stack(stack, symbol);

    $$ = create_node_with_type($2, data_type);
    add_child($$, create_node_with_value($1, symbol));
    add_child($$, $3);
  }
  | TK_IDENTIFICADOR TK_OC_LE literal ',' variable_list 
  { 
    SymbolTableItemValue symbol = create_symbol_table_value_with_type(SYMBOL_NATURE_VARIABLE, $1, data_type);
    add_value_to_table_on_stack(stack, symbol);

    $$ = create_node_with_type($2, data_type);
    add_child($$, create_node_with_value($1, symbol));
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($4);
  }
  ;

variable_assignment: TK_IDENTIFICADOR '=' expression
  {
    SymbolTableItemValue symbol = find_variable_value_in_stack_by_lexical_value(stack, $1);

    validate_variable_use(symbol, $1);

    Node* variable = create_node_with_type($1, symbol.type);
    $$ = create_node_from_assignment($2, variable, $3);

    add_child($$, variable);
    add_child($$, $3);
  }
  ;

function_call: 
  TK_IDENTIFICADOR '(' list_of_arguments ')'
  {
    SymbolTableItemValue symbol = find_function_value_in_stack_by_lexical_value(stack, $1);

    validate_function_call(symbol, $1);

    $$ = create_function_call_node_with_value($1, symbol);
    add_child($$, $3);
    free_lexical_value($2);
    free_lexical_value($4);
  }
  | TK_IDENTIFICADOR '(' ')'
  {
    SymbolTableItemValue symbol = find_function_value_in_stack_by_lexical_value(stack, $1);

    validate_function_call(symbol, $1);

    $$ = create_function_call_node_with_value($1, symbol);
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
    $$ = create_node_from_unary_operator($1, $2);
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
    $$ = create_node_from_unary_operator($1, $3);
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($2);
  }
  | TK_PR_IF '(' expression end_if_else_statement block TK_PR_ELSE block
  {
    $$ = create_node_from_unary_operator($1, $3);
    add_child($$, $3);
    add_child($$, $5);
    add_child($$, $7);
    free_lexical_value($2);
    free_lexical_value($6);
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
    $$ = create_node_from_unary_operator($1, $3);
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($2);
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
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_7_operators
  {
    $$ = $1;
  }
  ;

precedence_7_operators:
  precedence_7_operators TK_OC_AND precedence_6_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_6_operators
  {
    $$ = $1;
  }
  ;

precedence_6_operators:
  precedence_6_operators TK_OC_EQ precedence_5_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_6_operators TK_OC_NE precedence_5_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_5_operators
  {
    $$ = $1;
  }
  ;

precedence_5_operators:
  precedence_5_operators '<' precedence_4_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_5_operators '>' precedence_4_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_5_operators TK_OC_LE precedence_4_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_5_operators TK_OC_GE precedence_4_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_4_operators
  {
    $$ = $1;
  }
  ;

precedence_4_operators:
  precedence_4_operators '+' precedence_3_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_4_operators '-' precedence_3_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_3_operators
  {
    $$ = $1;
  }
  ;

precedence_3_operators:
  precedence_3_operators '*' precedence_2_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_3_operators '/' precedence_2_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_3_operators '%' precedence_2_operators
  {
    $$ = create_node_from_binary_operator($2, $1, $3);
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
    $$ = create_node_from_unary_operator($1, $2);
    add_child($$, $2);
  }
  | '!' operand
  {
    $$ = create_node_from_unary_operator($1, $2);
    add_child($$, $2);
  }
  | operand
  {
    $$ = $1;
  }
  ;

operand:
  TK_IDENTIFICADOR
  {
    SymbolTableItemValue symbol = find_variable_value_in_stack_by_lexical_value(stack, $1);

    validate_variable_use(symbol, $1);

    $$ = create_node_with_type($1, symbol.type);
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