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

rules: rules inline_command ';';
rules: inline_command ';';


/* >> Single Line Commands << */
inline_command: variable_declaration;
inline_command: variable_assignment;
inline_command: function_call;
inline_command: return_command;


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

%%

void yyerror (char const *message) {
	printf("line %d: [%s]\n", get_line_number(), message);
}