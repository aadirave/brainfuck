#include <stdbool.h>

#ifndef __UTILS_H__
#define __UTILS_H__

typedef enum {
  SYM_EOF = '\0',
  DEC_PTR = '<',
  INC_PTR = '>',
  INC_CELL = '+',
  DEC_CELL = '-',
  OUTPUT = '.',
  INPUT = ',',
  LOOP_OPEN = '[',
  LOOP_CLOSE = ']',
  ERROR = '&',
} Symbols;

typedef struct {
  Symbols sym;
  int repeat;
} Token;

typedef struct {
  char *program;
  Token *tok_list;

  int len;
  int pos;

  int *matching_bracket;
} Lexer;

bool preprocess_matching_brackets(Lexer *lex);
Lexer *make_lex(char *filename);
Token next(Lexer *lex);
void free_lexer(Lexer *lex);

#endif
