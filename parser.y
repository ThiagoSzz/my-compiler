%{
  #include <stdio.h>

  int yylex(void);
  void yyerror (char const *message);
  int get_line_number();
%}

%define parse.error verbose

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_ERRO

%%

///////////////////////////////
/* -->>     Program     <<-- */
///////////////////////////////

program:
  list_of_functions;
  | ;

list_of_functions:
  list_of_functions function;
  | function;

///////////////////////////////
/* -->>     Function    <<-- */
///////////////////////////////

function: function_signature function_body;

function_signature: TK_IDENTIFICADOR '=' list_of_parameters '>' type;

list_of_parameters:
  list_of_parameters TK_OC_OR parameter;
  | parameter;
  | ;

parameter: TK_IDENTIFICADOR '<''-' type;

function_body: block;

///////////////////////////////
/* -->>  Command Block  <<-- */
///////////////////////////////

block: 
  '{' commands '}';
  | '{' '}';

commands:
  commands simple_command;
  | simple_command;

///////////////////////////////
/* -->> Simple Commands <<-- */
///////////////////////////////

simple_command: 
  variable_declaration end_of_command;
  | variable_assignment end_of_command;
  | flow_control_statements end_of_command;
  | return_command end_of_command;
  | block end_of_command;
  | function_call end_of_command;

end_of_command: ';';

type: 
  TK_PR_INT;
  | TK_PR_FLOAT;

literal: 
  TK_LIT_INT;
  | TK_LIT_FLOAT;

variable_declaration: type variable_list;

variable_list:
  variable_list ',' declaration;
  | declaration;

declaration:
  TK_IDENTIFICADOR TK_OC_LE literal;
  | TK_IDENTIFICADOR;

variable_assignment: TK_IDENTIFICADOR '=' expression;

function_call: 
  TK_IDENTIFICADOR '(' list_of_arguments ')';
  | TK_IDENTIFICADOR '(' ')';

list_of_arguments:
  list_of_arguments ',' expression;
  | expression;

return_command: TK_PR_RETURN expression;

flow_control_statements:
  if_else_statement;
  | while_statement;

if_else_statement: 
  TK_PR_IF '(' expression ')' block;
  | if_else_statement TK_PR_ELSE block;

while_statement: TK_PR_WHILE '(' expression ')' block;

///////////////////////////////
/* -->>   Expressions   <<-- */
///////////////////////////////

expression: precedence_8_operators;

precedence_8_operators:
  precedence_8_operators TK_OC_OR precedence_7_operators;
  | precedence_7_operators;

precedence_7_operators:
  precedence_7_operators TK_OC_AND precedence_6_operators;
  | precedence_6_operators;

precedence_6_operators:
  precedence_6_operators TK_OC_EQ precedence_5_operators;
  | precedence_6_operators TK_OC_NE precedence_5_operators;
  | precedence_5_operators;

precedence_5_operators:
  precedence_5_operators '<' precedence_4_operators;
  | precedence_5_operators '>' precedence_4_operators;
  | precedence_5_operators TK_OC_LE precedence_4_operators;
  | precedence_5_operators TK_OC_GE precedence_4_operators;
  | precedence_4_operators;

precedence_4_operators:
  precedence_4_operators '+' precedence_3_operators;
  | precedence_4_operators '-' precedence_3_operators;
  | precedence_3_operators;

precedence_3_operators:
  precedence_3_operators '*' precedence_2_operators;
  | precedence_3_operators '/' precedence_2_operators;
  | precedence_3_operators '%' precedence_2_operators;
  | precedence_2_operators;

precedence_2_operators:
  precedence_1_minus_operator operand;
  | precedence_1_negation_operator operand;
  | operand;

precedence_1_minus_operator:
  precedence_1_minus_operator '-';
  | '-';

precedence_1_negation_operator:
  precedence_1_negation_operator '!';
  | '!';

operand:
  TK_IDENTIFICADOR;
  | literal;
  | function_call;
  | '(' expression ')';

///////////////////////////////
/* -->>   End Program   <<-- */
///////////////////////////////

%%

void yyerror (char const *message) {
	printf("line %d: [%s]\n", get_line_number(), message);
}