#ifndef PLC_LIST_H
#define PLC_LIST_H

#include <stdlib.h>

typedef struct node {
  struct node *next;
  void *value;
  size_t size;
} node_t;

typedef struct {
  node_t *head;
  int length;
} list_t;

list_t *list_create();
void list_release(list_t *list);

void list_pushhead(list_t *list, void *value, size_t value_size);
void *list_pophead(list_t *list);

typedef struct {
  list_t *list;
  node_t *current;
  int index;
} list_iterator_t;

list_iterator_t *list_iterator_create(list_t *list);
void list_iterator_release(list_iterator_t *iterator);
int list_iterator_next(list_iterator_t *iterator, void *value);
void list_iterator_reset(list_iterator_t *iterator);
#endif
