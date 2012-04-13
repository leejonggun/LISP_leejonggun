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

//共有する変数定義	共有する変数を定義するときはexternはつけない。
char *sym_data;			//String token.
char operater_data;		//operater type.
int num_data;			//number token.
int jindex;
node_t *root;			//indicates '(' node.
node_t *prev_list;		//indicates previous list of list_node.
node_t *open_node;		//indicates previous list of OPEN node

//関数定義	共有する関数を定義するときはexternをつける。
extern node_t* tokenize(const char* input, int tmp);				
//extern enum token_type symbol(const char *string, int last);	//symbol関数は読み込んだ文字列を、mallocで確保して、sym_dataに格納する。
//extern enum token_type number(const char *string, int first);	//number関数は読み込んだ数字の最初のindexを渡して、num_dataへ格納する。
//extern enum token_type operater(const char *string, int index);//operater関数はOPEN,CLOSE,OPERATER専用で文字をoperater_dataへ格納する。
extern void make_node(enum token_type tt, int length);
extern void print_node(node_t *node);
extern void free_node(node_t *node);
#endif
