#ifndef LISP_H
#define LISP_H

enum token_type {OPEN, CLOSE, NUMBER, SYMBOL, OPERATOR};
typedef struct node_t{			//Make tree construction
	enum token_type tt;			//The type of token
	union{
		int number;				//The data of number
		char *character;		//The data of string
		struct node_t *car;		//To Atom. atom has Data. List has the pointer to Atom.
	};
	struct node_t *cdr;			//To List. *cdr in Atom is NULL. Only has List *cdr.
} node_t;

//���L����ϐ���`	���L����ϐ����`����Ƃ���extern�͂��Ȃ��B
extern char *sym_data;			//String token.
extern char operater_data;		//operater type.
extern int num_data;			//number token.
extern int jindex;				//string index.
extern node_t *root;			//indicates '(' node.
extern node_t *open_node;		//indicates previous list of OPEN node

//�֐���`	���L����֐����`����Ƃ���extern������B
extern node_t* tokenize (const char* input);				
extern void print_node (node_t *node);
extern void free_node (node_t *node);
//extern node_t* eval (node_t *node);
#endif
