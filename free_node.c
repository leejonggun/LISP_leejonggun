#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

void free_node(node_t *node){
	switch(node->tt) {
		case OPEN:
			if(node->car != NULL){
				free_node(node->car);
			}else if(node->car == NULL){
				printf("Something wrong. list has something pointer.\n");
				break;
			}
			if(node->cdr != NULL){
				free_node(node->cdr);
			}else if(node->cdr == NULL){
				printf("Something wrong.Please check make_node\n");
				break;
			}
			free(node);
			break;
		case CLOSE:
			free(node);
			break;
		case OPERATOR:
			free(node->character);
			free(node);
			break;
		case NUMBER:
			free(node);
			break;
		case SYMBOL:
			free(node->character);
			free(node);
			break;
		case COMP:
			free(node->character);
			free(node);
			break;
		default:
			printf("ERROR in free_node: exception occured.\n");
			break;
	}
}
