#include "ast.h"
#include "io_interface.h"
#include "list.h"
#include "parser.h"
#include "scanner.h"
#include <stdlib.h>
#include <string.h>

parser_t *parser_create() {
  parser_t *parser = (parser_t *) malloc(sizeof(parser_t));
  parser->scanner = scanner_create();
  parser->context = list_create();

  return parser;
}

void parser_release(parser_t *parser) {
  scanner_release(parser->scanner);
  list_release(parser->context);
  free(parser);
}

ast_t *parser_abstraction(parser_t *, io_interface_t *);
ast_t *parser_application(parser_t *, io_interface_t *);
ast_t *parser_variable(parser_t *, io_interface_t *);

ast_t *parser_run(parser_t *parser, io_interface_t *io_interface) {
  if(scanner_peektoken(parser->scanner, io_interface)->type == T_BSLASH) {
    return parser_abstraction(parser, io_interface);
  } else if(scanner_peektoken(parser->scanner, io_interface)->type == T_LPAREN) {
    return parser_application(parser, io_interface);
  } else if(scanner_peektoken(parser->scanner, io_interface)->type == T_IDENTIFIER) {
    return parser_variable(parser, io_interface);
  }

  return NULL;
}

ast_t *parser_abstraction(parser_t *parser, io_interface_t *io_interface) {
  token_t *bslash_token = scanner_gettoken(parser->scanner, io_interface);

  token_t *parameter_token = scanner_gettoken(parser->scanner, io_interface);
  if(parameter_token->type != T_IDENTIFIER) {
    token_release(parameter_token);
    token_release(bslash_token);
    return NULL;
  }

  size_t identifier_size = strlen(parameter_token->identifier) + 1;
  list_pushhead(parser->context, parameter_token->identifier, identifier_size);

  token_t *period_token = scanner_gettoken(parser->scanner, io_interface);
  if(period_token->type != T_PERIOD) {
    token_release(period_token);
    free(list_pophead(parser->context));
    token_release(parameter_token);
    token_release(bslash_token);
    return NULL;
  }

  ast_t *body_ast = parser_run(parser, io_interface);
  if(body_ast == NULL) {
    token_release(period_token);
    free(list_pophead(parser->context));
    token_release(parameter_token);
    token_release(bslash_token);
    return NULL;
  }

  token_release(period_token);
  free(list_pophead(parser->context));
  token_release(parameter_token);
  token_release(bslash_token);
  return ast_create(A_ABSTRACTION, body_ast);
}

ast_t *parser_application(parser_t *parser, io_interface_t *io_interface) {
  token_t *lparen_token = scanner_gettoken(parser->scanner, io_interface);

  ast_t *function_ast = parser_run(parser, io_interface);
  if(function_ast == NULL) {
    token_release(lparen_token);
    return NULL;
  }

  ast_t *argument_ast = parser_run(parser, io_interface);
  if(argument_ast == NULL) {
    token_release(lparen_token);
    ast_release(function_ast, R_ALL);
    return NULL;
  }

  token_t *rparen_token = scanner_gettoken(parser->scanner, io_interface);
  if(rparen_token->type != T_RPAREN) {
    token_release(lparen_token);
    ast_release(function_ast, R_ALL);
    ast_release(argument_ast, R_ALL);
    return NULL;
  }

  token_release(rparen_token);
  token_release(lparen_token);
  return ast_create(A_APPLICATION, function_ast, argument_ast);
}

ast_t *parser_variable(parser_t *parser, io_interface_t *io_interface) {
  token_t *identifier_token = scanner_gettoken(parser->scanner, io_interface);
  char identifier[10];
  int index;
  list_iterator_t *iterator = list_iterator_create(parser->context);
  while((index = list_iterator_next(iterator, identifier)) != -1) {
    if(strcmp(identifier, identifier_token->identifier) == 0) {
      break;
    }
  }

  if(index == -1) {
    list_iterator_release(iterator);
    token_release(identifier_token);
    return NULL;
  }

  list_iterator_release(iterator);
  token_release(identifier_token);
  return ast_create(A_VARIABLE, index);
}
