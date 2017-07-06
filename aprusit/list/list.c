/******************************************************************************
list.c

Implementation of simple linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "list.h"

/* list_new: return a new list structure */
List *list_new(void)
{
	List *l;

	l = (List *) malloc(sizeof(List));
	l->len = 0;

	/* insert root element which should never be removed */
	l->first = l->last = (Node *) malloc(sizeof(Node));
	l->first->elm = NULL;
	l->first->next = NULL;
	return l;
}

/* Clean up */
void list_delete(List *l) {
	Node *n;
	n = l->first;
	while (n != NULL) {
		free(n->elm);
		Node *next;
		next = n->next;
		free(n);
		n = next;
	}
	free(l);
}

/* list_add: add node n to list l as the last element */
void list_add(List *l, Node *n)
{
	pthread_mutex_lock(&(l->mutex));
	l->last->next = n;
	l->last = n; //Assign new node as last in list
	l->len++;
	pthread_mutex_unlock(&(l->mutex));
}

/* list_remove: remove and return the first (non-root) element from list l */
Node *list_remove(List *l)
{
	Node *n;
	if (l->len == 0) {
		printf("The list is empty. \n");
		return NULL;
	} else {
		pthread_mutex_lock(&(l->mutex));
		n = l->first->next; //The first (non-root) node
		if (l->len == 1) {
			/* Empty the list */
			l->last = l->first; //Root node
		}
		l->first->next = n->next; //Point to NULL if len == 1
		l->len--;
		pthread_mutex_unlock(&(l->mutex));
	}
	return n; //Memory allocated for this should be freed by caller
}

/* Free memory for a node */
void node_destroy(Node *n) {
	free(n->elm);
	free(n);
}

/* node_new: return a new node structure */
Node *node_new(void)
{
	Node *n;
	n = (Node *) malloc(sizeof(Node));
	n->elm = NULL;
	n->next = NULL;
	return n;
}

/* node_new_str: return a new node structure, where elm points to new copy of s */
Node *node_new_str(char *s)
{
	Node *n;
	n = (Node *) malloc(sizeof(Node));
	n->elm = (void *) malloc((strlen(s)+1) * sizeof(char));
	strcpy((char *) n->elm, s);
	n->next = NULL;
	return n;
}

/* node_new_str: return a new node structure, where elm points a to an integer num */
Node *node_new_int(int *num)
{
	Node *n;
	n = (Node *) malloc(sizeof(Node));
	n->elm = (void *) malloc(sizeof(int*));
	n->elm = num;
	n->next = NULL;
	return n;
}

/* Prints the structure of the FIFO linked list */
void print_structure(List *l) {
	Node *n;
	for (int i = 1; i <= l->len; i++) {
		if (i == 1) {
			n = l->first->next;
			printf("root -> ");
		} else {
			n = n->next;
		}
		printf("('%s') -> ", (char*) n->elm);
		if (i == l->len) {
			printf("end\n");
		}
	}
}
