#include <stdlib.h>
#include "list.h"
#include "util.h"

List *List_new(freeFunction freeFunction) {
    List *ls = malloc(sizeof(List));
    check_mem(ls);
    ls->head = NULL;
    ls->tail = NULL;
    ls->freeFunction = freeFunction;
    ls->size = 0;
    return ls;
}

void List_append(List *list, void *data) {
    _Node *new = malloc(sizeof(_Node));
    check_mem(new);
    new->data = data;
    new->next = NULL;

    if(!list->head) {
        list->head = new;
        list->tail = new;
    } else {
        list->tail->next = new;
        list->tail = new;
    }

    list->size += 1;
}

void List_prepend(List *list, void *data) {
    _Node *new = malloc(sizeof(_Node));
    check_mem(new);
    new->data = data;
    new->next = NULL;

    if(!list->head) {
        list->head = new;
        list->tail = new;
    } else {
        new->next = list->head;
        list->head = new;
    }

    list->size += 1;
}

void *List_head(List *list) {
    if (!list->head) {
        return NULL;
    } else {
        return list->head->data;
    }
}

void *List_pop(List *list) {
    if (!list->head) {
        return NULL;
    } else if (list->head->next == NULL) { // List of size 1
        void *data = list->head->data;
        list->head = list->tail = NULL;
        list->size -= 1;
        return data;
    } else {
        void *data = list->head->data;
        list->head = list->head->next;
        list->size -= 1;
        return data;
    }
}

void List_free(List *list) {
    _Node *n = list->head;
    while (n != NULL) {
        _Node *next = n->next;
        if (list->freeFunction != NULL) {
            list->freeFunction(n->data);
        }
        free(n);
        n = next;
    }
    free(list);
}

int List_size(List *list) {
    return list->size;
}

int List_empty(List *list) {
    return list->head == NULL && list->tail == NULL;
}
