#ifndef PLC_AST_H
#define PLC_AST_H

typedef enum {
  A_VARIABLE,
  A_ABSTRACTION,
  A_APPLICATION,
} ast_type_t;

typedef struct ast {
  ast_type_t type;
  struct ast **children;
  int variable_index;
} ast_t;

ast_t *ast_create(ast_type_t type, ...);
void ast_release(ast_t *ast);
void ast_print(ast_t *ast);


#endif
