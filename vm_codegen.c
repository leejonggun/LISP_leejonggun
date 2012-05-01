#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

/*
typedef struct opline_t {
	enum op_type type;
	union {
	struct opline_t *op_T;
	int op;
	};
	union {
	struct opline_t *op_F;
	struct opline_t *next;
	};
}opline_t
*/
//enum op_type type = { PUSH, POP, ADD, SUB, MUL, DIV, SML, BIG, EQL, IF, END};
node_t *vm_open = NULL;
static opline_t* func_new ();
static opline_t* type_divari ( node_t *node );
static opline_t* func_smaller ( node_t *node );
static opline_t* func_bigger ( node_t *node );
static opline_t* func_equal ( node_t *node );
static opline_t* func_push ( node_t *node );
static opline_t* func_add ( node_t *node );
static opline_t* func_sub ( node_t *node );
static opline_t* func_mul ( node_t *node );
static opline_t* func_div ( node_t *node );
static opline_t* func_if ( node_t *node );
static opline_t* func_defun ( node_t *vm_open );
static opline_t* func_end ();

/*vm生成部分*/
opline_t *codegen ( node_t *node ) {
	node_t *vm_root = node;//The root->car has some function.
	node_t *vm_open = node;//The OPEN parenthesis.
	opline_t *vm_top = NULL;//the root of cons tree.
	opline_t *vm_chain = NULL;
	while (vm_open->tt != CLOSE) {
		if (vm_open->car->tt == OPERATOR || vm_open->car->tt == COMP ) {//演算子が来た場合。分岐の仕方->演算子が来たら次のトークン(数字)を読む。それからは数字と演算子をセットで考える。
			//最初の*carの場合。(最初に括弧が来ても計算対応)
			vm_open = vm_open->cdr;//演算子を飛ばして最初の引数(数字)から読む。
			vm_top = type_divari (vm_open);
			vm_chain = vm_top;
			while (vm_chain->next != NULL) {//入れ子になっている場合、最後の命令に進める。その続きから命令をつなげる。
				vm_chain = vm_chain->next;
			}
			vm_open = vm_open->cdr;
			while (vm_open->tt != CLOSE) {
				vm_chain->next = type_divari (vm_open);
				while (vm_chain->next != NULL) {//入れ子になっている場合、最後の命令に進める。その続きから命令をつなげる。
					vm_chain = vm_chain->next;
				}
				if ( vm_open->cdr != NULL ) {//演算子命令を挿入。
					vm_chain->next = type_divari (vm_root);
					vm_chain = vm_chain->next;
				}
				vm_open = vm_open->cdr;
			}
		} else if (vm_open->car->tt == SYMBOL) {
			char *vm_func = vm_open->car->character;
			/*if文(if (cond) (op_T) (op_F))*/			
			if (strcmp (vm_func, "if") == 0) {
				vm_open = vm_open->cdr;
				vm_top = type_divari (vm_open);//条件式をvm_topにPUSHする。
				opline_t *vm_chain = vm_top;
				while ( vm_chain->next != NULL ) {//条件式の最後から命令列をつなげてく。
					vm_chain = vm_chain->next;
				}
				opline_t *condition = type_divari (vm_root);//conditionはop_Tとop_F(条件式が真の場合と偽の場合に実行される命令)を持っている。
				vm_chain->next = condition;
				/*条件式が真の時の命令*/
				/*条件式が偽の時の命令*/
				//上はそれぞれfunc_ifの中で命令列を作る。
				return vm_top;
			/*TODO defun文(defun func_name (args) (function))*/
			} else if (strcmp (vm_func, "defun") == 0) {
				opline_t *function_top = func_defun (vm_open);
				return function_top;
			} else {//関数の呼び出し。今はまだエラー処理
				//node_t *vm_variable = hash_search ( defun_table, vm_open );
				printf("You don't define the function.\n");
				return NULL;
			}
		} else {//今の場合、エラー処理。
			printf("This is not good....\n");
			return NULL;
		}
	}
	//vm_openがcloseなら最初の演算子を読む。
	return vm_top;
}
/*vm生成部分*/
//opline_tを新しく作る関数。
static opline_t* func_new () {
	opline_t *func_new = (opline_t*) malloc ( sizeof (opline_t) );
	func_new->type = PUSH;
	func_new->op = 0;
	func_new->next = NULL;
	return func_new;
}
//node_tのtt (OPEN, CLOSE, NUMBER, SYMBOL, COMP)を受け付ける関数。
static opline_t* type_divari ( node_t *node ) {
	opline_t *func_result = NULL;
	switch (node->car->tt) {
		case OPEN:
			func_result = codegen (node->car);
			return func_result;
		case NUMBER:
			func_result = func_push ( node );
			return func_result;
		case SYMBOL:
			if ( string_cmp (node->car, "if") == 0) {
				func_result = func_if ( node );
				printf("func_result='%p, %d, %p, %p'\n",func_result,func_result->type,func_result->op_T,func_result->op_F);
				return func_result;
			}// else {
			//	func_result = new_vm ();
			//	strcpy (func_result->viriable, node->car->character);
			//	func_result->op = 0;
			//}
		case OPERATOR:
			switch (node->car->character[0]) {
				case '+':
					func_result = func_add ( node );
					return func_result;
				case '-':
					func_result = func_sub ( node );
					return func_result;
				case '*':
					func_result = func_mul ( node );
					return func_result;
				case '/':
					func_result = func_div ( node );
					return func_result;
			}
		case COMP:
			switch (node->car->character[0]) {
				case '<':
					func_result = func_smaller ( node );
					return func_result;
				case '>':
					func_result = func_bigger ( node );
					return func_result;
				case '=':
					func_result = func_equal ( node );
					return func_result;
			}
		default:
			printf("You got nothing.\n");
			return func_result;
	}
}
//typeによって処理を変える->PUSH関数。POP関数。ADD関数。
static opline_t* func_push ( node_t *node ) {
	opline_t *vm = func_new ();
	vm->type = PUSH;
	vm->op = node->car->number;
	vm->next = NULL;
	return vm;
}
static opline_t* func_smaller ( node_t *node ) {
	opline_t *sml_vm = func_new ();
	sml_vm->type = SML;
	sml_vm->op = 0;
	sml_vm->next = NULL;
	return sml_vm;
	//実行は別にする。
}
static opline_t* func_bigger ( node_t *node ) {
	opline_t *big_vm = func_new ();
	big_vm->type = BIG;
	big_vm->op = 0;
	big_vm->next = NULL;
	return big_vm;
	//実行は別にする。
}
static opline_t* func_equal ( node_t *node ) {
	opline_t *eql_vm = func_new ();
	eql_vm->type = EQL;
	eql_vm->op = 0;
	eql_vm->next = NULL;
	return eql_vm;
	//実行は別にする。
}
static opline_t* func_add ( node_t *node ) {
	opline_t *add_vm = func_new ();
	add_vm->type = ADD;
	add_vm->op = 0;
	add_vm->next = NULL;
	return add_vm;
	//実行は別にする。
}
static opline_t* func_sub ( node_t *node ) {
	opline_t *sub_vm = func_new ();
	sub_vm->type = SUB;
	sub_vm->op = 0;
	sub_vm->next = NULL;
	return sub_vm;
	//実行は別にする。
}
static opline_t* func_mul ( node_t *node ) {
	opline_t *mul_vm = func_new ();
	mul_vm->type = MUL;
	mul_vm->op = 0;
	mul_vm->next = NULL;
	return mul_vm;
	//実行は別にする。
}
static opline_t* func_div ( node_t *node ) {
	opline_t *div_vm = func_new ();
	div_vm->type = DIV;
	div_vm->op = 0;
	div_vm->next = NULL;
	return div_vm;
	//実行は別にする。
}
static opline_t* func_if ( node_t *node ) {
	opline_t *if_vm = func_new ();
	if_vm->type = IF;
	node_t *T_node = node->cdr->cdr;
	if_vm->op_T = type_divari (T_node);

	opline_t *tmp_vm = if_vm->op_T;//Make tmp_vm
	while (tmp_vm->next != NULL) {
		tmp_vm = tmp_vm->next;
	}
	tmp_vm->next = func_end ();
	tmp_vm = tmp_vm->next;//tmp_vm->type is "END".

	node_t *F_node = node->cdr->cdr->cdr;
	if_vm->op_F = type_divari (F_node);
	
	tmp_vm->next = if_vm->op_F;// After "END", op_F has come.
	while (tmp_vm->next != NULL) {
		tmp_vm = tmp_vm->next;
	}
	tmp_vm->next = func_end ();

	return if_vm;
}
static opline_t* func_end () {
	opline_t *end_vm = func_new ();
	end_vm->type = END;
	end_vm->op = 0;
	end_vm->next = NULL;
	return end_vm;
}
static opline_t *func_defun ( node_t *vm_open ) {
	node_t *func_name = vm_open->cdr;
	node_t *func_args = vm_open->cdr->cdr;
	node_t *function = vm_open->cdr->cdr->cdr->car;
	if (defun_table == NULL) {
		defun_table = (hash_table_t*) malloc (sizeof (hash_table_t) );
		defun_table->func_codegen = (opline_t*) malloc (sizeof (opline_t) );
	}
	hash_set (defun_table, func_name, func_args);//hash_tableにセット完了。
	printf("log: SUCCESS to set defun_table->entry[1]->key='%s'\n",defun_table->entry[1]->key);
	defun_table->func_codegen = codegen ( function );//defun_tableの関数の命令列を作成。
	return defun_table->func_codegen; 
}
//static opline_t *defun_codegen ( node_t *function ) {
//	/*関数の変数が来た場合。node->car->characterが関数に使われる変数。*/
//	func_result->type = PUSH;
//	func_result->next = NULL;
//	return func_result;
//}
