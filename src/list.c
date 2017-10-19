#include "list.h"
#include <stdlib.h>
#include <string.h>

list_t *list_create() {
  list_t *list = (list_t *) malloc(sizeof(list_t));

  list->head = NULL;
  list->length = 0;

  return list;
}

void list_release(list_t *list) {
  while(list->length > 0) {
    free(list_pophead(list));
  }
  free(list);
}

void list_pushhead(list_t *list, void *value, size_t value_size) {
  node_t *node = (node_t *) malloc(sizeof(node_t));

  node->value = malloc(value_size);
  memcpy(node->value, value, value_size);
  node->size = value_size;
  node->next = list->head;
  list->head = node;

  ++list->length;
}

void *list_pophead(list_t *list) {
  if(list->length == 0) {
    return NULL;
  }

  node_t *node = list->head;
  list->head = node->next;
  --list->length;

  void *value = node->value;
  free(node);

  return value;
}

list_iterator_t *list_iterator_create(list_t *list) {

  list_iterator_t *iterator = (list_iterator_t *) malloc(sizeof(list_iterator_t));
  iterator->list = list;
  iterator->current = list->head;
  iterator->index = 0;

  return iterator;
}

void list_iterator_release(list_iterator_t *iterator) {
  free(iterator);
}

int list_iterator_next(list_iterator_t *iterator, void *value) {
  if(iterator->current != NULL) {
    memcpy(value, iterator->current->value, iterator->current->size);
    iterator->current = iterator->current->next;
    ++iterator->index;
    return iterator->index - 1;
  }

  return -1;
}

void list_iterator_reset(list_iterator_t *iterator) {
  iterator->current = iterator->list->head;
  iterator->index = 0;
}
