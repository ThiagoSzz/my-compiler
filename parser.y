/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

%{
  #include <stdio.h>

  #include "ast.h"
  #include "lexical_value.h"

  int yylex(void);
  void yyerror (char const *mensagem);
  int get_line_number();

  extern void *tree;
%}

%union
{
  LexicalValue LexicalValue;
  struct Node* Node;
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
%type<Node> type
%type<Node> literal
%type<Node> function
%type<Node> function_signature
%type<Node> list_of_parameters
%type<Node> parameter
%type<Node> function_body
%type<Node> block
%type<Node> commands
%type<Node> simple_command
%type<Node> end_of_command
%type<Node> variable_declaration
%type<Node> variable_list
%type<Node> declaration
%type<Node> variable_assignment
%type<Node> function_call
%type<Node> list_of_arguments
%type<Node> return_command
%type<Node> flow_control_statements
%type<Node> if_else_statement
%type<Node> while_statement
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
    $$ = NULL;
    free_lexical_value($1);
  }
  | TK_PR_FLOAT
  {
    $$ = NULL;
    free_lexical_value($1);
  }
  ;

///////////////////////////////
/* -->>     Literal     <<-- */
///////////////////////////////

literal: 
  TK_LIT_INT
  {
    $$ = create_node($1);
  }
  | TK_LIT_FLOAT
  {
    $$ = create_node($1);
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
    $$ = create_node($1);
    free_lexical_value($2);
    free_lexical_value($4);
  }
  ;

list_of_parameters:
  parameter TK_OC_OR list_of_parameters
  {
    $$ = $3;
    free_lexical_value($2);
  }
  | parameter
  {
    $$ = $1;
  }
  | 
  {
    $$ = NULL;
  }
  ;

parameter: TK_IDENTIFICADOR '<''-' type
  {
    $$ = NULL;
    free_lexical_value($1);
    free_lexical_value($2);
    free_lexical_value($3);
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
  '{' commands '}'
  {
    $$ = $2;
    free_lexical_value($1);
    free_lexical_value($3);
  }
  | '{' '}'
  {
    $$ = NULL;
    free_lexical_value($1);
    free_lexical_value($2);
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
  declaration ',' variable_list  
  {
    if ($1) {
      $$ = $1;
      add_child($$, $3);
    } else {
      $$ = $3;
    }
    free_lexical_value($2);
  }
  | declaration
  {
    $$ = $1;
  }
  ;

declaration:
  TK_IDENTIFICADOR TK_OC_LE literal
  {
    $$ = create_node($2);
    add_child($$, create_node($1));
    add_child($$, $3);
  }
  | TK_IDENTIFICADOR
  {
    $$ = NULL;
    free_lexical_value($1);
  }
  ;

variable_assignment: TK_IDENTIFICADOR '=' expression
  {
    $$ = create_node($2);
    add_child($$, create_node($1));
    add_child($$, $3);
  }
  ;

function_call: 
  TK_IDENTIFICADOR '(' list_of_arguments ')'
  {
    $$ = create_function_call_node($1);
    add_child($$, $3);
    free_lexical_value($2);
    free_lexical_value($4);
  }
  | TK_IDENTIFICADOR '(' ')'
  {
    $$ = create_function_call_node($1);
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
    $$ = create_node($1);
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
  TK_PR_IF '(' expression ')' block
  {
    $$ = create_node($1);
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($2);
    free_lexical_value($4);
  }
  | TK_PR_IF '(' expression ')' block TK_PR_ELSE block
  {
    $$ = create_node($1);
    add_child($$, $3);
    add_child($$, $5);
    add_child($$, $7);
    free_lexical_value($2);
    free_lexical_value($4);
    free_lexical_value($6);
  }
  ;

while_statement: TK_PR_WHILE '(' expression ')' block
  {
    $$ = create_node($1);
    add_child($$, $3);
    add_child($$, $5);
    free_lexical_value($2);
    free_lexical_value($4);
  }
  ;

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
    $$ = create_node($2);
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
    $$ = create_node($2);
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
    $$ = create_node($2);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_6_operators TK_OC_NE precedence_5_operators
  {
    $$ = create_node($2);
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
    $$ = create_node($2);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_5_operators '>' precedence_4_operators
  {
    $$ = create_node($2);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_5_operators TK_OC_LE precedence_4_operators
  {
    $$ = create_node($2);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_5_operators TK_OC_GE precedence_4_operators
  {
    $$ = create_node($2);
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
    $$ = create_node($2);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_4_operators '-' precedence_3_operators
  {
    $$ = create_node($2);
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
    $$ = create_node($2);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_3_operators '/' precedence_2_operators
  {
    $$ = create_node($2);
    add_child($$, $1);
    add_child($$, $3);
  }
  | precedence_3_operators '%' precedence_2_operators
  {
    $$ = create_node($2);
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
    $$ = create_node($1);
    add_child($$, $2);
  }
  | '!' operand
  {
    $$ = create_node($1);
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
    $$ = create_node($1);
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
  printf("line %d: [%s]\n", get_line_number(), message);
}