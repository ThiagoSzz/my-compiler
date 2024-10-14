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

/* >> Program Rules << */
program: ;
program: rules;

end_of_statement: ';';

rules: rules inline_command end_of_statement;
rules: inline_command end_of_statement;


/* >> Single Line Commands << */
inline_command: variable_declaration;
inline_command: variable_assignment;
inline_command: function_call;
inline_command: return_command;
inline_command: flow_control_statements;


/* >> Types << */
type: 
    TK_PR_INT;
  | TK_PR_FLOAT;


/* >> Literals << */
literal: 
    TK_LIT_INT;
  | TK_LIT_FLOAT;


/* >> Expressions (TBD) << */
expression: ;


/* >> Command Block << */
command_block: block_rules;
block_rules: 
    '{' block_expressions '}';
  | '{' '}'
block_expressions:
    block_expressions expression end_of_statement
  | expression end_of_statement;


/* >> Variable Declaration << */
variable_declaration: type var_dec_rules;
var_dec_rules:
    var_dec_rules ',' var_dec_expression;
  | var_dec_expression;
var_dec_expression:
    TK_IDENTIFICADOR TK_OC_LE literal;
  | TK_IDENTIFICADOR;


/* >> Variable Assignment << */
variable_assignment: TK_IDENTIFICADOR '=' expression;


/* >> Function call << */
function_call: TK_IDENTIFICADOR '(' func_call_argument ')';
func_call_argument:
    expression;
  | func_call_argument ',' expression;


/* >> Return command << */
return_command: TK_PR_RETURN expression;


/* >> Flow control statements << */
flow_control_statements: flow_ctrl_rules;
flow_ctrl_rules: 
    TK_PR_IF '(' expression ')' command_block
  | flow_ctrl_rules TK_PR_ELSE command_block;


%%

void yyerror (char const *message) {
	printf("line %d: [%s]\n", get_line_number(), message);
}