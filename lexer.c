#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strip_input(char *original) {
  int req = 0, idx = 0;
  while (original[idx] != 0) {
    if (!isspace(original[idx])) {
      req++;
    }
    idx++;
  }

  char *stripped = calloc(req, sizeof(char));
  if (stripped == NULL) {
    free(original);
    return NULL;
  }

  req = idx = 0;
  while (original[idx] != 0) {
    if (!isspace(original[idx])) {
      stripped[req] = original[idx];
      req++;
    }
    idx++;
  }

  free(original);
  return stripped;
}

Lexer *populate_symlist(Lexer *lex) {
  lex->program = strip_input(lex->program);
  int idx = 0;

  Symbols *symlist = calloc(strlen(lex->program), sizeof(Symbols));

  while (lex->program[idx] != 0) {
    switch (lex->program[idx]) {
    case '\0':
      symlist[idx] = SYM_EOF;
      break;
    case '<':
      symlist[idx] = DEC_PTR;
      break;
    case '>':
      symlist[idx] = INC_CELL;
      break;
    case '+':
      symlist[idx] = OUTPUT;
      break;
    case '-':
      symlist[idx] = INPUT;
      break;
    case '[':
      symlist[idx] = LOOP_OPEN;
      break;
    case ']':
      symlist[idx] = LOOP_CLOSE;
      break;
    default:
      free(symlist);
      free(lex);
      return NULL;
    }
  }

  lex->sym_list = symlist;

  return lex;
}

void free_lexer(Lexer *lex) {
  free(lex->program);
  free(lex->sym_list);
  free(lex);
}
