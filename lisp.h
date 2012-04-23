#ifndef LISP_H
#define LISP_H

#define HASH_SIZE 4

enum token_type {OPEN, CLOSE, NUMBER, SYMBOL, OPERATOR, COMP};
typedef struct node_t{			//Make tree construction
	enum token_type tt;			//The type of token
	union{
		int number;				//The data of number
		char *character;		//The data of string
		struct node_t *car;		//To Atom. atom has Data. List has the pointer to Atom.
	};
	struct node_t *cdr;			//To List. *cdr in Atom is NULL. Only has List *cdr.
} node_t;
typedef struct hash_entry_t {
	    const char *key;
		    node_t *value;
			    /* list */
			struct hash_entry_t *next;
} hash_entry_t;
typedef struct hash_table_t {
	    hash_entry_t* entry[HASH_SIZE];
		    /* stack */
		    struct hash_table_t *prev;
}hash_table_t;

//共有する変数定義	共有する変数の宣言はそれぞれ必要なところでする。ここは定義するだけ。
extern char *sym_data;			//String token.
extern char operater_data;		//operater type.
extern int num_data;			//number token.
extern int jindex;				//string index.
extern int Comp_flag;			//Distinguish Compare and Calculate
extern int func_call_flag;
extern node_t *root;			//indicates '(' node.
extern node_t *open_node;		//indicates previous list of OPEN node
extern hash_table_t *setq_table;
extern hash_table_t *defun_table;

//関数定義	共有する関数を定義するときはexternをつける。
extern node_t* tokenize (const char* input);				
extern void print_node (node_t *node);
extern void free_node (node_t *node);
extern node_t *copy_node (node_t *node);
extern int eval (node_t *node);
extern void hash_set (hash_table_t *table, node_t *key, node_t *value);
extern node_t *hash_search (hash_table_t *table, node_t *node);
extern int length_cdr (node_t *first_OpenNode);
#endif
