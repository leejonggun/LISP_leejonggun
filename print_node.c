#include <stdio.h>
#include <string.h>
#include "lisp.h"


void print_node(node_t *node) {		//node = root. root indicates '('node.
	switch(node->tt) {
		case OPEN:
			printf("OPEN:node, node->car, node->cdr = '%p, %p, %p'\n",node,node->car,node->cdr);
			if(node->car != NULL){
				print_node(node->car);
			}else if(node->car == NULL){
				printf("please input something after '('\n");
				break;
			}
			if(node->cdr != NULL){
				print_node(node->cdr);
			}else if(node->cdr == NULL){
				printf("The tree construction does not work.\n");
				break;
			}
			break;
		case CLOSE:
			printf("CLOSE:node = '%p'\n",node);
			break;
		case OPERATOR:

			printf("OPERATER:node, node->character = '%p, %s'\n",node,node->character);
			break;
		case NUMBER:
			printf("NUMBER:node, node->number = '%p, %d'\n",node,node->number);
			break;
		case SYMBOL:
			printf("SYMBOL:node, node->character = '%p, %s'\n",node,node->character);
			break;
		default:
			printf("ERROR:in print_node: exception(This node has no token_type tt\n");
			break;
		}
}
