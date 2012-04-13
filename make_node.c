#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"


node_t* make_list(){
	node_t *list = (node_t*)malloc(sizeof(node_t));
	list->tt = OPEN;
	return list;
}
//node_t *prev_list = NULL;			//indicates previous List  node.

void make_node(enum token_type tt, int token_length){
	node_t *node = (node_t*)malloc(sizeof(node_t));
	node_t *list;			//for List_node
	node->tt = tt;

	if(tt == OPEN){
		if(prev_list != NULL){
			printf("Second OPEN\n");
			prev_list->car = node;		//listの*carに途中のOPEN nodeアドレスを入れる。
			list = make_list();
			prev_list->cdr = list;		//listの*cdrには次のlistのアドレスを入れる。
			open_node = prev_list;		//CLOSE nodeが来たときに返すopen_nodeを更新。
			prev_list = node;			//pre_list を 途中のOPEN nodeアドレスに更新。
		}else{
			printf("first OPEN\n");
			open_node = node;
			prev_list = node;		//tree construction starts from prev_list, '('. *prev_list is global pointer for list_node.
			root = node;
		}
	}else if(tt == CLOSE){
		free(node);
		prev_list->tt = tt;
		prev_list->car = NULL;
		prev_list->cdr = NULL;			//The last Atom node has no cdr and car.
	}else if(tt == OPERATER){		//Atom_node
		node->cdr = NULL;
		node->character = &operater_data;//node->character got operater.
		printf("log_operater = '%s'\n",node->character);
		prev_list->car = node;			//prev_list has the address of '(' node. The Atom_node's address be into prev_list->car.
		list = make_list();			//make list_node for next token.
		prev_list->cdr = list;			//	connect '(' token to list_nodei as stack
		prev_list = list;				//list address be into prev_list. prev_list works as previous list_node in next loop.  prev_list have a role as top of stack.
	
	}else if(tt == NUMBER){			//Atom
		node->cdr = NULL;			//Atom has no cdr.
		node->number = num_data;	//copy num_data.
		printf("log_number = '%d'\n",node->number);
		prev_list->car = node;			//list_node connect to Atom_node
		list = make_list();
		prev_list->cdr = list;			//connect previous list_node to new list_node.
		prev_list = list;
	
	}else if(tt == SYMBOL){			//Call Atom function
		node->cdr = NULL;
		node->character = (char*)malloc(sizeof(token_length));	//j means sym_data's length.
		int i = 0;
		for(i = 0; i <= token_length; i++){			// copy sym_data.
		node->character[i] = sym_data[i];
		}
		printf("log_symbol = '%p'\n",node->character);
		prev_list->car = node;
		list = make_list();
		prev_list->cdr = list;
		prev_list = list;
	}else{
		printf("error make_node\n");
	}
}

