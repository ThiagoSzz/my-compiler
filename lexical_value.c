#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexical_value.h"

// create LexicalValue
LexicalValue create_lexical_value(char *value, TokenTypeEnum token_type, LiteralTypeEnum literal_type, int line_number)
{
  LexicalValue lexical_value;
  lexical_value.line_number = line_number;
  lexical_value.label = strdup(value);
  lexical_value.token_type = token_type;
  lexical_value.literal_type = literal_type;

  switch (literal_type)
  {
    case INT:
      lexical_value.value.int_value = atoi(value);
      break;
    case FLOAT:
      lexical_value.value.float_value = atof(value);
      break;
    case NOT_A_LITERAL:
      break;
  }

  return lexical_value;
}

// free LexicalValue
void free_lexical_value(LexicalValue lexical_value)
{
  if (lexical_value.label)
  {
    free(lexical_value.label);
  }
}