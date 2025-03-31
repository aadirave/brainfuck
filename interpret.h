#include "lexer.h"

#ifndef __INTERPRET_H__
#define __INTERPRET_H__

typedef struct {
  Lexer *lexed;
  char *registers;
  long len;
  long current_pos;
} Interpreter;

void free_interpreter(Interpreter *interpret);

#endif
