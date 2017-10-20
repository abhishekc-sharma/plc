#ifndef PLC_AST_H
#define PLC_AST_H

typedef enum {
  A_VARIABLE,
  A_ABSTRACTION,
  A_APPLICATION,
} ast_type_t;

typedef enum {
  R_ALL,
  R_EXCEPT_0,
  R_EXCEPT_1
} ast_release_t;

typedef struct ast {
  ast_type_t type;
  struct ast **children;
  int variable_index;
} ast_t;

ast_t *ast_create(ast_type_t type, ...);
void ast_release(ast_t *ast, ast_release_t release_type);
void ast_print(ast_t *ast);
ast_t *ast_clone(ast_t *ast);

#endif
