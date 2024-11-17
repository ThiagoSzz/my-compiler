#ifndef LEXICAL_VALUE_H
#define LEXICAL_VALUE_H

typedef enum
{
  IDENTIFIER,
  LITERAL,
  NOT_RELEVANT
} TokenTypeEnum;

typedef enum
{
  INT,
  FLOAT,
  NOT_A_LITERAL
} LiteralTypeEnum;

typedef struct
{
  union
  {
    int int_value;
    float float_value;
  } value;
  TokenTypeEnum token_type;
  LiteralTypeEnum literal_type;
  int line_number;
  char *label;
} LexicalValue;

LexicalValue create_lexical_value(char *value, TokenTypeEnum token_type, LiteralTypeEnum literal_type, int line_number);

void free_lexical_value(LexicalValue lexical_value);

#endif