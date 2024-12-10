/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

%{
  #include "ast.h"
  #include "lexical_value.h"
  #include "parser.tab.h"

  #define YYSTYPE LexicalValue

  int get_line_number(void);
  char *get_value(void);
%}

%option noyywrap
%option yylineno

%%

"int" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_PR_INT; }
"float" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_PR_FLOAT; }
"if" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_PR_IF; }
"else" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_PR_ELSE; }
"while" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_PR_WHILE; }
"return" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_PR_RETURN; }

[0-9]*\.[0-9]+ { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_LITERAL, LITERAL_TYPE_FLOAT, get_line_number()); return TK_LIT_FLOAT; }
[0-9]+ { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_LITERAL, LITERAL_TYPE_INT, get_line_number()); return TK_LIT_INT; }

[a-z_][a-z0-9_]* { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_IDENTIFIER, NOT_A_LITERAL, get_line_number()); return TK_IDENTIFICADOR; }

"<=" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_OC_LE; }
">=" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_OC_GE; }
"==" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_OC_EQ; }
"!=" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_OC_NE; }
"&" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_OC_AND; }
"|" { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return TK_OC_OR; }

[!|*|/|%|+|\-|<|>|{|}|(|)|=|,|;] { yylval.LexicalValue = create_lexical_value(get_value(), TOKEN_TYPE_NOT_RELEVANT, NOT_A_LITERAL, get_line_number()); return yytext[0]; }

\n { }
[ \t] { }
"//".*\n* { }

. { return TK_ERRO; }

%%

int get_line_number(void) {
  return yylineno;
}

char *get_value(void) {
  return yytext;
}