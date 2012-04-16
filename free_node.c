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
				printf("Something wrong.Please check make_node");
				break;
			}
			free(node);
//			printf("IN free_node node, node->car ='%p, %p'\n",node,node->car);
//			printf("Free_node:sym_data ='%p'\n",sym_data);
			break;
		case CLOSE:
			free(node);
			break;
		case OPERATER:
			free(node);
			break;
		case NUMBER:
			free(node);
//			printf("IN free_node:node->number = '%d'\n",node->number);
			break;
		case SYMBOL:
			free(node->character);
			free(node);
			break;
		default:
			printf("ERROR in free_node: exception occured.");
			break;
	}
}
