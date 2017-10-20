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

void ast_release(ast_t *ast, ast_release_t release_type) {
  switch(ast->type) {
    case A_VARIABLE:
      free(ast);
      break;
    case A_ABSTRACTION:
      release_type == R_EXCEPT_0 ? 0 : ast_release(ast->children[0], R_ALL);
      free(ast->children);
      free(ast);
      break;
    case A_APPLICATION:
      release_type == R_EXCEPT_0 ? 0 : ast_release(ast->children[0], R_ALL);
      release_type == R_EXCEPT_1 ? 0 : ast_release(ast->children[1], R_ALL);
      free(ast->children);
      free(ast);
      break;
    default:
      break;
  }
}

ast_t *ast_clone(ast_t *ast) {
  ast_t *clone = NULL;
  switch(ast->type) {
    case A_VARIABLE:
      ;
      clone = ast_create(A_VARIABLE, ast->variable_index);
      break;
    case A_ABSTRACTION:
      ;
      ast_t *body_ast = ast_clone(ast->children[0]);
      clone = ast_create(A_ABSTRACTION, body_ast);
      break;
    case A_APPLICATION:
      ;
      ast_t *function_ast = ast_clone(ast->children[0]);
      ast_t *argument_ast = ast_clone(ast->children[1]);
      clone = ast_create(A_APPLICATION, function_ast, argument_ast);
      break;
  }

  return clone;
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
