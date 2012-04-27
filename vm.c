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
//enum op_type type = { PUSH, POP, ADD, SUB, MUL, DIV, SML, BIG, EQL};
node_t *vm_open = NULL;
static opline_t* new_vm ();
void free_vm ( opline_t *vm ); 
static opline_t* type_divari ( node_t *node );
static opline_t* func_smaller ( node_t *node );
static opline_t* func_bigger ( node_t *node );
static opline_t* func_equal ( node_t *node );
static opline_t* func_push ( node_t *node );
static opline_t* func_add ( node_t *node );
static opline_t* func_sub ( node_t *node );
static opline_t* func_mul ( node_t *node );
static opline_t* func_div ( node_t *node );

/*vm生成部分*/
opline_t *codegen ( node_t *node ) {
	node_t *vm_root = node;//The root->car has some function.
	node_t *vm_open = node;//The OPEN parenthesis.
	opline_t *vm_top = NULL;//the root of cons tree.
	while (vm_open->tt != CLOSE) {
		if (vm_open->car->tt == OPERATOR || vm_open->car->tt == COMP ) {//演算子が来た場合。分岐の仕方->演算子が来たら次のトークン(数字)を読む。それからは数字と演算子をセットで考える。
			//最初の*carの場合。
			vm_open = vm_open->cdr;//演算子を飛ばして最初の引数(数字)から読む。
			vm_top = func_push (vm_open);
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
		} else if (vm_open->car->tt == SYMBOL) {
			char *vm_func = vm_open->car->character;
			/*if文*/			
			if ( strcmp (vm_func, "if") == 0) {
				vm_open = vm_open->cdr;
				opline_t *condition = type_divari (vm_open);//条件式をPUSHする。
				//conditionはTかNilで返ってくる。
				if ( condition->op_T != NULL ) {
				printf("How are you?\n");
				} else if ( condition->op_F != NULL ) {
				printf("I'm fine.\n");
				}
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
static opline_t* new_vm () {
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
	//	case CLOSE:
	//		return func_result; 
		case NUMBER:
			func_result = func_push ( node );
			return func_result;
		case SYMBOL:
			if ( string_cmp (node->car, "if") ) {
				func_result = func_if ( node );
				return func_result;
			}
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
	opline_t *vm = new_vm ();
	vm->type = PUSH;
	vm->op = node->car->number;
	vm->next = NULL;
	return vm;
}
static opline_t* func_smaller ( node_t *node ) {
	opline_t *cmp_result = new_vm ();
	cmp_result->type = SML;
	cmp_result->op = 0;
	cmp_result->next = NULL;
	return cmp_result;
	//実行は別にする。
}
static opline_t* func_bigger ( node_t *node ) {
	opline_t *cmp_result = new_vm ();
	cmp_result->type = BIG;
	cmp_result->op = 0;
	cmp_result->next = NULL;
	return cmp_result;
	//実行は別にする。
}
static opline_t* func_equal ( node_t *node ) {
	opline_t *cmp_result = new_vm ();
	cmp_result->type = EQL;
	cmp_result->op = 0;
	cmp_result->next = NULL;
	return cmp_result;
	//実行は別にする。
}
static opline_t* func_add ( node_t *node ) {
	opline_t *add_result = new_vm ();
	add_result->type = ADD;
	add_result->op = 0;
	add_result->next = NULL;
	return add_result;
	//実行は別にする。
}
static opline_t* func_sub ( node_t *node ) {
	opline_t *sub_result = new_vm ();
	sub_result->type = SUB;
	sub_result->op = 0;
	sub_result->next = NULL;
	return sub_result;
	//実行は別にする。
}
static opline_t* func_mul ( node_t *node ) {
	opline_t *mul_result = new_vm ();
	mul_result->type = MUL;
	mul_result->op = 0;
	mul_result->next = NULL;
	return mul_result;
	//実行は別にする。
}
static opline_t* func_div ( node_t *node ) {
	opline_t *div_result = new_vm ();
	div_result->type = DIV;
	div_result->op = 0;
	div_result->next = NULL;
	return div_result;
	//実行は別にする。
}
static opline_t* func_if ( node_t *node ) {

}
