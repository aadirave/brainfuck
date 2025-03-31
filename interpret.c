#include "interpret.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static void run_brainfuck_helper(Interpreter *interp);

void free_interpreter(Interpreter *interpret) {
  free_lexer(interpret->lexed);
  free(interpret->registers);
  free(interpret);
}

Interpreter *init_from_lex(Lexer *lex) {
  Interpreter *interp = calloc(1, sizeof(Interpreter));
  if (interp == NULL) {
    free_lexer(lex);
    return NULL;
  }

  interp->lexed = lex;

  // start with 32 registers, and realloc if needed
  interp->len = 32;
  interp->registers = calloc(interp->len, sizeof(unsigned char));

  return interp;
}

void run_brainfuck(Lexer *lex) {
  Interpreter *interp = init_from_lex(lex);
  if (interp == NULL) {
    return;
  }

  if (interp->lexed == NULL) {
    free(interp->registers);
    free(interp);
    return;
  }

  run_brainfuck_helper(interp);

  free_interpreter(interp);
}

static void run_brainfuck_helper(Interpreter *interp) {
  if (interp == NULL || interp->current_pos < 0) {
    interp->current_pos = 0;
    return;
  }

  if (interp->lexed == NULL) {
    return;
  }

  if (interp->lexed->pos >= interp->lexed->len) {
    return;
  }

  if (interp->current_pos >= interp->len) {
    interp->registers =
        realloc(interp->registers, interp->len * sizeof(unsigned char) * 2);
    if (interp->registers == NULL) {
      return;
    }
  }

  // now, deal with current symbol
  Lexer *lex = interp->lexed;

  // TODO: deal with loops here

  switch (lex->tok_list[lex->pos].sym) {
  case SYM_EOF:
  case ERROR:
    return;
  case LOOP_OPEN:
    if (interp->registers[interp->current_pos] == 0) {
      lex->pos = lex->matching_bracket[lex->pos]; // Jump to matching `]`
    }
    break;
  case LOOP_CLOSE:
    if (interp->registers[interp->current_pos] != 0) {
      lex->pos = lex->matching_bracket[lex->pos]; // Jump back to `[`
    }
    break;
  case DEC_PTR:
    interp->current_pos -= lex->tok_list[lex->pos].repeat;
    break;
  case INC_PTR:
    interp->current_pos += lex->tok_list[lex->pos].repeat;
    break;
  case INC_CELL:
    interp->registers[interp->current_pos] += lex->tok_list[lex->pos].repeat;
    break;
  case DEC_CELL:
    interp->registers[interp->current_pos] -= lex->tok_list[lex->pos].repeat;
    break;
  case OUTPUT:
    fprintf(stdout, "%d", interp->registers[interp->current_pos]);
    break;
  case INPUT:
    fscanf(stdin, "%c", &(interp->registers[interp->current_pos]));
    break;
  default:
    return;
  }
  lex->pos++;

  run_brainfuck_helper(interp);
}
