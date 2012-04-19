#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lisp.h"

node_t *copy_node (node_t *node) {
	node_t *clone = (node_t *) malloc (sizeof (node_t) );
	switch (node->tt) {
		case OPEN:
			clone->tt = node->tt;
			if (node->car != NULL) {
				clone->car = copy_node (node->car);
			} else if ( node->car == NULL ) {
				printf("ERROR1:copy_node in hash.c: Open_node has no car!\n");
				return NULL;
			}
			if (node->cdr != NULL ) {
				clone->cdr = copy_node (node->cdr);
			} else if (node->cdr == NULL ) {
				printf("ERROR2:copy_node in hash.c: Open_node has no cdr!\n");
				return NULL;
			}
			return clone;
		case CLOSE:
			clone->tt = node->tt;
			clone->cdr = node->cdr;
			clone->car = node->car;
			return clone;
		case NUMBER:
			clone->tt = node->tt;
			clone->cdr = node->cdr;
			clone->number = node->number;
			return clone;
		case SYMBOL:
			clone->tt = node->tt;
			clone->cdr = node->cdr;
			clone->character = (char *) malloc (sizeof (strlen (node->character) +1) );
			memcpy(clone->character, node->character, strlen(node->character)+1);
			return clone;
		case OPERATOR:
			clone->tt = node->tt;
			clone->cdr = node->cdr;
			clone->character = (char *) malloc (sizeof (2));
			memcpy(clone->character, node->character, 2);
			return clone;
		case COMP:
			clone->tt = node->tt;
			clone->cdr = node->cdr;
			clone->character = (char *) malloc (sizeof (2) );
			memcpy(clone->character, node->character, 2);
			return clone;
		default:
			printf("ERROR:copy_node in hash.c: Something wrong.\n");
			return NULL;
	}
}
