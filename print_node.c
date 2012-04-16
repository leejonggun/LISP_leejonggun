#include <stdio.h>
#include <string.h>
#include "lisp.h"


void print_node(node_t *node) {		//node = root. root indicates '('node.
	switch(node->tt) {
		case OPEN:
			printf("node, node->car, node->cdr = '%p, %p, %p'\n",node,node->car,node->cdr);
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
			printf("CLOSE:node->car, node->cdr = '%p, %p'\n",node->car,node->cdr);
			break;
		case OPERATOR:
			printf("OPERATER:*node->character, node->cdr = '%c, %p'\n",*node->character,node->cdr);
			break;
		case NUMBER:
			printf("NUMBER:node->number, node->cdr = '%d, %p'\n",node->number,node->cdr);
			break;
		case SYMBOL:
			printf("SYMBOL:node->character, node->cdr = '%s, %p'\n",node->character,node->cdr);
			break;
		default:
			printf("ERROR: exception(This node has no token_type tt\n");
			break;
/*  		if(node->car != NULL){
				print(node->car);
			}else if(node->car == NULL){
				printf("This list node has no data pointer?\n");
				break;
			}
			if(node->cdr != NULL){
				printf("LIST:node->car, node->cdr = '%p, %p'\n",node->car,node->cdr);
				print(node->cdr);
			}else if(node->cdr == NULL){
				printf("Sonething wrong for CLOSE node\n");
				break;
			}
*/
		}
}
