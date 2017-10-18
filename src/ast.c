#include "ast.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

ast_t *ast_create(ast_type_t type, ...) {
  ast_t *ast = (ast_t *) malloc(sizeof(ast_t));
  ast->type = type;

  va_list ap;
  va_start(ap, type);
  switch(type) {
    case A_VARIABLE:
      ast->children = NULL;
      ast->variable_index = va_arg(ap, int);
      break;
    case A_ABSTRACTION:
      ast->children = (ast_t **) malloc(sizeof(ast_t *));
      ast->children[0] = va_arg(ap, ast_t *);
      ast->variable_index = -1;
      break;
    case A_APPLICATION:
      ast->children = (ast_t **) malloc(2 * sizeof(ast_t *));
      ast->children[0] = va_arg(ap, ast_t *);
      ast->children[1] = va_arg(ap, ast_t *);
      ast->variable_index = -1;
      break;
    default:
      break;
  }

  va_end(ap);
  return ast;
}

void ast_release(ast_t *ast) {
  switch(ast->type) {
    case A_VARIABLE:
      free(ast);
      break;
    case A_ABSTRACTION:
      ast_release(ast->children[0]);
      free(ast->children);
      free(ast);
      break;
    case A_APPLICATION:
      ast_release(ast->children[0]);
      ast_release(ast->children[1]);
      free(ast->children);
      free(ast);
      break;
    default:
      break;
  }
}

void print_indent(int level) {
  while(level-- > 0) printf("\t");
}

void ast_printl(ast_t *ast, int level) {
  switch(ast->type) {
    case A_VARIABLE:
      print_indent(level);
      printf("%d\n", ast->variable_index);
      break;
    case A_ABSTRACTION:
      print_indent(level);
      printf("\\.\n");
      ast_printl(ast->children[0], level + 1);
      break;
    case A_APPLICATION:
      print_indent(level);
      printf("(\n");
      ast_printl(ast->children[0], level + 1);
      printf("\n");
      ast_printl(ast->children[1], level + 1);
      print_indent(level);
      printf(")\n");
      break;
    default:
      break;
  }
}

void ast_print(ast_t *ast) {
  ast_printl(ast, 0);
}
