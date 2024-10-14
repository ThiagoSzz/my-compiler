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

/* -->> Program Rules <<-- */
program: 
  rules;
  | ;

end_of_statement: ';';

rules: 
  rules inline_command end_of_statement;
  | inline_command end_of_statement;


/* -->> Single Line Commands <<-- */
inline_command: 
  variable_declaration;
  | variable_assignment;
  | function_call;
  | return_command;
  | flow_control_statements;


/* -->> Types <<-- */
type: 
  TK_PR_INT;
  | TK_PR_FLOAT;


/* -->> Literals <<-- */
literal: 
  TK_LIT_INT;
  | TK_LIT_FLOAT;


/* -->> Expressions <<-- */
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


/* -->> Command Block (TBD) <<-- */
command_block: 
  '{' block_expressions '}';
  | '{' '}';

block_expressions:
  block_expressions expression end_of_statement
  | expression end_of_statement;


/* -->> Variable Declaration <<-- */
variable_declaration: type variable_declaration_rules;

variable_declaration_rules:
  variable_declaration_rules ',' variable_declaration_statement;
  | variable_declaration_statement;

variable_declaration_statement:
  TK_IDENTIFICADOR TK_OC_LE literal;
  | TK_IDENTIFICADOR;


/* -->> Variable Assignment <<-- */
variable_assignment: TK_IDENTIFICADOR '=' expression;


/* -->> Function call <<-- */
function_call: 
  TK_IDENTIFICADOR '(' function_call_argument ')';
  | TK_IDENTIFICADOR '(' ')';

function_call_argument:
  expression;
  | function_call_argument ',' expression;


/* -->> Return command <<-- */
return_command: TK_PR_RETURN expression;


/* -->> Flow control statements <<-- */
flow_control_statements:
  if_else_statement;
  | while_statement;

if_else_statement: 
  TK_PR_IF '(' expression ')' command_block;
  | if_else_statement TK_PR_ELSE command_block;

while_statement: TK_PR_WHILE '(' expression ')' command_block;


%%

void yyerror (char const *message) {
	printf("line %d: [%s]\n", get_line_number(), message);
}