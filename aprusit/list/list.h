/******************************************************************************
list.h

Header file with definition of a simple linked list.

******************************************************************************/
#include <pthread.h>

#ifndef _LIST_H
#define _LIST_H

/* structures */
typedef struct node {
	void *elm; /* use void type for generality; we cast the element's type to void type */
	struct node *next;
} Node;

typedef struct list {
	int len;
	Node *first;
	Node *last;
	pthread_mutex_t mutex;
} List;

/* functions */
List *list_new(void);            /* return a new list structure */
void list_delete(List *l);       /* delete a list structure */
void list_add(List *l, Node *n); /* add node n to list l as the last element */
Node *list_remove(List *l);      /* remove and return the first element from list l*/
Node *node_new(void);            /* return a new node structure */
Node *node_new_str(char *s);     /* return a new node structure, where elm points to new copy of string s */
Node *node_new_int(int *num);    /* return a new node structure, where elm points to a copy of  num */
void node_destroy(Node *n);      /* destroy a node and reclaim memory  */
void print_structure(List *l);   /* Print graphical structure of list to stdout */

#endif
