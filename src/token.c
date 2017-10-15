#include <stdlib.h>
#include <string.h>
#include "token.h"

token_t *token_create(token_type_t type, long long line, long long column, char *identifier) {
  token_t *token = (token_t *) malloc(sizeof(token_t));

  token->type = type;
  token->line = line;
  token->column = column;

  int length = strlen(identifier);
  token->identifier = (char *) malloc(sizeof(char) * (length + 1));
  strncpy(token->identifier, identifier, length + 1);

  return token;
}

void token_release(token_t *token) {
  free(token->identifier);
  free(token);
}
