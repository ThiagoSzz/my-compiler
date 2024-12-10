/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexical_value.h"

// create lexical value for regex
LexicalValue create_lexical_value(char *value, TokenTypeEnum token_type, LiteralTypeEnum literal_type, int line_number)
{
  LexicalValue lexical_value;
  lexical_value.label = strdup(value);
  lexical_value.line_number = line_number;
  lexical_value.token_type = token_type;
  lexical_value.literal_type = literal_type;

  // convert value to int or float
  // NOT_A_LITERAL added to prevent warning
  switch (literal_type)
  {
  case LITERAL_TYPE_INT:
    lexical_value.value.int_value = atoi(value);
    break;
  case LITERAL_TYPE_FLOAT:
    lexical_value.value.float_value = atof(value);
    break;
  case NOT_A_LITERAL:
    break;
  }

  return lexical_value;
}

// free lexical value
void free_lexical_value(LexicalValue lexical_value)
{
  if (lexical_value.label)
  {
    free(lexical_value.label);
  }
}