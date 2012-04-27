#include <stdio.h>
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
//enum op_type type = { PUSH, POP, ADD };
node_t *vm_open = NULL;
opline_t* new_vm ();
opline_t* func_first_vm ( node_t *node );
void free_vm ( opline_t *vm ); 
opline_t* type_divari ( node_t *node );
opline_t* func_push ( node_t *node );
opline_t* func_pop ();
opline_t* func_add ( node_t *node );

/*vm生成部分*/
opline_t *codegen ( node_t *node ) {
	node_t *vm_root = node;//The root->car has some function.
	node_t *vm_open = node;//The OPEN parenthesis.
	opline_t *vm_top = NULL;//the root of cons tree.
	while (vm_open->tt != CLOSE) {
		if (vm_open->car->tt == OPERATOR ) {//演算子が来た場合。分岐の仕方->演算子が来たら次のトークン(数字)を読む。それからは数字と演算子をセットで考える。
			//最初の*carの場合。
			vm_open = vm_open->cdr;//演算子を飛ばして最初の引数(数字)から読む。
			vm_top = func_first_vm (vm_open);
			opline_t *vm_chain = vm_top;
			printf("log1: vm_top->op ='%d'\n",vm_top->op);
			vm_open = vm_open->cdr;
			
			while (vm_open->tt != CLOSE) {
				vm_chain->next = type_divari (vm_open);
				while (vm_chain->next != NULL) {//入れ子になっている場合、最後の命令に進める。その続きから命令をつなげる。
					vm_chain = vm_chain->next;
				}
				printf("log2: vm_chain->op ='%d'\n",vm_chain->op);
				if ( vm_open->cdr != NULL ) {
					vm_chain->next = type_divari (vm_root);
					vm_chain = vm_chain->next;
			}
				vm_open = vm_open->cdr;
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
//最初の処理だけ特別処理。
opline_t* func_first_vm ( node_t *node ) {//nodeは最初の'('。node->cdr->car->numberは最初の数字。
	opline_t *first = new_vm ();
	first->type = PUSH;
	first->op = node->car->number;
	first->next = NULL;
	return first;
}
//opline_tを新しく作る関数。
opline_t* new_vm () {
	opline_t *new_vm = (opline_t*) malloc ( sizeof (opline_t) );
	new_vm->type = PUSH;
	new_vm->op = 0;
	new_vm->next = NULL;
	return new_vm;
}
//opline_tをフリーする。
void free_vm ( opline_t *vm ) {
	free( vm );
}
//node_tのtt (OPEN, CLOSE, NUMBER, SYMBOL, COMP)を受け付ける関数。
opline_t* type_divari ( node_t *node ) {
	opline_t *func_result = NULL;
	switch (node->car->tt) {
		case OPEN:
			func_result = codegen (node->car);
			return func_result;
			//break;
	//	case CLOSE:
	//		return func_result; 
		case NUMBER:
			func_result = func_push ( node );
			return func_result;
			//break;
		case OPERATOR:
		case SYMBOL://This means only add.
			func_result = func_add ( node );
			return func_result;
			//break;
//		case COMP:
//			func_result = func_add ( node );
//			return func_result;
//			break;
		default:
			printf("You got nothing.\n");
			return func_result;
			//break;
	}
}
//typeによって処理を変える->PUSH関数。POP関数。ADD関数。
opline_t* func_push ( node_t *node ) {
	opline_t *vm = new_vm ();
	vm->type = PUSH;
	vm->op = node->car->number;
	vm->next = NULL;
	return vm;
}
//opline_t* func_pop (opline_t *opline) {
//	free(opline);
//	return vm_poped;
//}
opline_t* func_add ( node_t *node ) {
	opline_t *add_result = new_vm ();
	add_result->type = ADD;
	add_result->op = 0;
	add_result->next = NULL;
	return add_result;
	//実行は別にする。
	/*opline_t *first_args = func_pop ();
	opline_t *second_args = func_pop ();
	add_result->op = first_args->op + second_args->op;
	func_push ( add_result );*/
}
