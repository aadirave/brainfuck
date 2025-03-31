#include "lexer.h"

#ifndef __INTERPRET_H__
#define __INTERPRET_H__

typedef struct {
  Lexer *lexed;
  unsigned char *registers;
  long len;
  long current_pos;
} Interpreter;

Interpreter *init_from_lex(Lexer *lex);
void run_brainfuck(Lexer *lex);
void free_interpreter(Interpreter *interpret);

#endif
