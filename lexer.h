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
} Symbols;

typedef struct {
  Symbols sym;
  int repeat;
} Token;

typedef struct {
  char *program;
  Symbols *sym_list;
  int pos;
} Lexer;

#endif
