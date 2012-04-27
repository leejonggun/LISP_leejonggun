#include <stdio.h>
#include <stdlib.h>
#include "lisp.h"

/*
typedef struct opline_t {
	enum op_type type;
	int op;
	struct opline_t *next;
}opline_t
*/
//enum op_type type = { PUSH, POP, ADD };
node_t *vm_open = NULL;
opline_t *vm_top = NULL;//the root of cons tree.
opline_t *bottom = NULL;//the end of cons tree.
opline_t* new_vm ();
opline_t* func_first_vm ( node_t *node );
void free_vm ( opline_t *vm ); 
opline_t* type_divari ( node_t *node );
void func_push ( node_t *node );
opline_t* func_pop ();
opline_t* func_add ( node_t *node );

/*vm生成部分*/
opline_t *make_vm ( node_t *node ) {
	node_t *vm_root = node;//The root->car has some function.
	node_t *vm_open = node;//The OPEN parenthesis.
	//opline_t *vm_first = NULL;
	while (vm_open->tt != CLOSE) {
		if (vm_open->car->tt == OPERATOR ) {//演算子が来た場合。分岐の仕方->演算子が来たら次のトークン(数字)を読む。それからは数字と演算子をセットで考える。
			if (vm_top == NULL) {//最初の*carの場合。
				vm_open = vm_open->cdr;//演算子を飛ばして最初の引数(数字)から読む。
				opline_t *first_vm = func_first_vm (vm_open);
				printf("log1: first_vm->op ='%d'\n",first_vm->op);
				printf("log1: vm_top->type ='%d'\n",vm_top->type);
		 	 	/*vm_top = type_divari (vm_open->car);
		  		printf("vm_top->type ='%d'\n", vm_top->type);
		  		vm_open = vm_open->cdr;*/
			}
			vm_open = vm_open->cdr;//vm_top != NULL の場合、演算子の次の数字にcdrを進める。
			//vm_first = func_first_vm (vm_open);
			//vm_open = vm_open->cdr;
			while (vm_open->tt != CLOSE) {
				opline_t *vm_calc = type_divari (vm_open);
				printf("log_operator: vm_calc->op, vm_calc->type='%d, %d'\n",vm_calc->op,vm_calc->type);
				if (vm_open->cdr->cdr != NULL) {
					vm_calc->next = type_divari (vm_root);
				}
				vm_open = vm_open->cdr;
			}
			
		} else {//今の場合、エラー処理。
			printf("This is not good....\n");
			return NULL;
		/*	opline_t *after_divari = type_divari (vm_open);
			printf("log2: after_divari='%p', after_divari->op='%d', after_divari->type = '%d'\n",after_divari,after_divari->op,after_divari->type);
			if ( vm_open->cdr->tt != CLOSE ) {
				after_divari->next = type_divari ( vm_open );//2回目からは数字を読んだ後、演算子を呼び出す。
			}
			vm_open = vm_open->cdr;
			}*/
		}
	}
	//vm_openがcloseなら最初の演算子を読む。
	opline_t *vm_close = type_divari (vm_root);//vm_open->carは演算子。
	printf("log3: vm_close='%p', vm_close(vm_root)->type='%d'\n",vm_close,vm_close->type);
	return vm_top;
}
/*vm生成部分*/
//最初の処理だけ特別処理。
opline_t* func_first_vm ( node_t *node ) {//nodeは最初の'('。node->cdr->car->numberは最初の数字。
	opline_t *first = new_vm ();
	first->type = PUSH;
	first->op = node->car->number;
	first->next = NULL;
	bottom = first;
	bottom->next = first;
	vm_top = first;
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
			func_result = make_vm (node->car);
			return func_result;
			//break;
	//	case CLOSE:
	//		return func_result; 
		case NUMBER:
			func_push ( node );
			func_result = bottom;//bottom is latest stack.
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
void func_push ( node_t *node ) {
	opline_t *vm = new_vm ();
	vm->type = PUSH;
	vm->op = node->car->number;
	bottom->next = vm;	//In this sentence, bottom is old one.
	bottom = vm;		//vm is current stack. bottom became the newest stack.
}
opline_t* func_pop () {
	opline_t *vm_poped = vm_top;//bottom is the newest.
	vm_top = vm_top->next;		//bottom became old one.
	return vm_poped;
}
opline_t* func_add ( node_t *node ) {
	opline_t *add_result = new_vm ();
	add_result->type = ADD;
	add_result->op = 0;
	bottom->next = add_result;
	bottom = add_result;
	return add_result;
	//実行は別にする。
	/*opline_t *first_args = func_pop ();
	opline_t *second_args = func_pop ();
	add_result->op = first_args->op + second_args->op;
	func_push ( add_result );*/
}
