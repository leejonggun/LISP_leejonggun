#ifndef LISP_H
#define LISP_H

#define HASH_SIZE 4
#define STACK_SIZE 1024

#define FLAG      1
#ifdef FLAG

#else

#endif
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

enum op_type {PUSH, PUSH_V, POP, ADD, SUB, MUL, DIV, SML, BIG, EQL, IF, END, CALL};
typedef struct opline_t {
	enum op_type type;
	int args_number;
	union {
		int op;
		char *viriable;
		struct opline_t *op_T;
	};
	union {
		struct opline_t *next;
		struct opline_t *op_F;
	};
}opline_t;

typedef struct hash_entry_t {
	    const char *key;
		union {
		    node_t *value;
			opline_t *vm_code;
		//	int arg_index;
		};
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
extern node_t *root;			//indicates '(' node.
extern node_t *open_node;		//indicates previous list of OPEN node
extern int Comp_flag;			//Distinguish Compare and Calculate
extern int func_call_flag;
extern int quit_flag;
extern hash_table_t *setq_table;//hash_table for setq function.
extern hash_table_t *defun_table;//hash_table for defun function.
extern hash_table_t *tmp_table;//hash_table for temprary use.
extern const char *type_name[];
extern int stack[STACK_SIZE]; 

//関数定義	共有する関数を定義するときはexternをつける。
extern void start (const char *input);
extern node_t* tokenize (const char* input);				
extern void print_node (node_t *node);
extern void free_node (node_t *node);
extern node_t *copy_node (node_t *node);
extern int eval (node_t *node);
extern int string_cmp (node_t *node, char *string);
extern void hash_set (hash_table_t *table, node_t *key, node_t *value);
extern node_t *hash_search (hash_table_t *table, node_t *node);
extern int add ( node_t *node );
extern int sub ( node_t *node );
extern int mul ( node_t *node );
extern int dir ( node_t *node );
extern int smaller ( node_t *node );
extern int bigger ( node_t *node );
extern int equal ( node_t *node );
extern opline_t *codegen ( node_t *node ); 
extern opline_t *vm_run ( opline_t *opline, int *stack);
extern void hash_set_vm ( hash_table_t *table, node_t *key, opline_t *function);
extern void hash_set_args (hash_table_t *table, node_t *key, int index);
extern opline_t *hash_search_vm ( hash_table_t *table, node_t *node );
#endif
