#ifndef __TOKEN_H
#define __TOKEN_H

typedef enum {
  T_BSLASH,
  T_PERIOD,
  T_LPAREN,
  T_RPAREN,
  T_IDENTIFIER,
  T_EOF
} token_type_t;

typedef struct {
  token_type_t type;
  long long line;
  long long column;
  char *identifier;
} token_t;

token_t *token_create(token_type_t type, long long line, long long column, char *identifier);
void token_release(token_t *token);


#endif
