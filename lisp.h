#ifndef LISP_H
#define LISP_H

enum token_type {OPEN, CLOSE, NUMBER, SYMBOL, OPERATER};
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
char *sym_data;			//String token.
char operater_data;		//operater type.
int num_data;			//number token.
int jindex;
node_t *root;			//indicates '(' node.
node_t *prev_list;		//indicates previous list of list_node.
node_t *open_node;		//indicates previous list of OPEN node

//�֐���`	���L����֐����`����Ƃ���extern������B
extern node_t* tokenize(const char* input, int tmp);				
//extern enum token_type symbol(const char *string, int last);	//symbol�֐��͓ǂݍ��񂾕�������Amalloc�Ŋm�ۂ��āAsym_data�Ɋi�[����B
//extern enum token_type number(const char *string, int first);	//number�֐��͓ǂݍ��񂾐����̍ŏ���index��n���āAnum_data�֊i�[����B
//extern enum token_type operater(const char *string, int index);//operater�֐���OPEN,CLOSE,OPERATER��p�ŕ�����operater_data�֊i�[����B
extern void make_node(enum token_type tt, int length);
extern void print_node(node_t *node);
extern void free_node(node_t *node);
#endif
