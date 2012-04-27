#include <stdio.h>
#include "lisp.h"

int add ( node_t *node );
int sub ( node_t *node );
int mul ( node_t *node );
int dir ( node_t *node );
int smaller ( node_t *node );
int bigger ( node_t *node );
int equal ( node_t *node );



int add ( node_t *node ) {
	int tmp_number = 0;
	int add_result = 0;
	//最初の引数(最初の数字)を設定。
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
	add_result = eval ( node->car );
	node = node->cdr;
	}
	//	ここでevalを呼ぶ。CLOSEまでwhileでループする。
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: add in eval.c\n");
			return (-1);
		}
		add_result += tmp_number;
		node = node->cdr;
	}
	return add_result;
}
int sub ( node_t *node ) {
	int tmp_number = 0;
	int sub_result = 0;
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
		sub_result = eval ( node->car );
		node= node->cdr;
	}
	while ( node->tt != CLOSE ) {
			if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: sub in eval.c\n");
			return (-1);
		}
		sub_result -=  tmp_number;
		node = node->cdr;
	}
	return sub_result;	
}
int mul ( node_t *node ) {
	int tmp_number = 0;
	int mul_result = 1;
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
		mul_result = eval ( node->car );
		node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: mul in eval.c\n");
			return (-1);
		}
		mul_result *= tmp_number;
		node = node->cdr;
	}
	return mul_result;
}
int dir ( node_t *node ) {
	int tmp_number = 0;
	int dir_result = 1;
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
		dir_result = eval ( node->car );
		node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: dir in eval.c\n");
			return (-1);
		}
		dir_result /= tmp_number;
		node = node->cdr;
	}
	return dir_result;
}
int smaller ( node_t *node ) {
	int first_number = 0;
	int tmp_number = 0;
	int comp_result = 0;	//1 means True and 0 means False. Tentatively the result is False.
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {//最初の引数をfirst_numberに格納する。
		first_number = eval ( node->car );
		node = node->cdr;
	}
	if ( node->tt == CLOSE ) {
		printf("ERROR:smaller in eval.c: You need two or more arguments!\n");
		return (-1);
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );	//2番目の引数からtmp_numberに格納して、比較をはじめる。
			if ( first_number < tmp_number ) {	//比較結果がTrueなら比較するnumberを次にすすめる。(< 1 2 3 ) がTrue. (< 1 4 2)はFalse.
				comp_result = 1;
				first_number = tmp_number;
			}
			else if ( first_number >= tmp_number ) {//比較結果がFalseならFalse ( = 0 )を返す。to do "Distinguish T, Nil between 1, 0"
				comp_result = 0;
				//printf("in smaller:first_number'%d'>=tmp_number'%d'\n",first_number,tmp_number);
				return comp_result;
			}
		} else {
			printf("ERROR: smaller in eval.c\n");
			return (-1);
		}
		node = node->cdr;
	}
	return comp_result;
}
int bigger ( node_t *node ) {
	int first_number = 0;
	int tmp_number = 0;
	int comp_result = 0;	//1 means True and 0 means False. Tentatively the result is False.
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {//最初の引数をfirst_numberに格納する。
		first_number = eval ( node->car );
		node = node->cdr;
	}
	if ( node->tt == CLOSE ) {
		printf("ERROR:bigger in eval.c: You need two or more arguments!\n");
		return (-1);
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );	//2番目の引数からtmp_numberに格納して、比較をはじめる。
			if ( first_number > tmp_number ) {	//比較結果がTrueなら比較するnumberを次にすすめる。(< 1 2 3 ) がTrue. (< 1 4 2)はFalse.
				comp_result = 1;
				first_number = tmp_number;
			}
			else if ( first_number <= tmp_number ) {//比較結果がFalseならFalse ( = 0 )を返す。to do "Distinguish T, Nil between 1, 0"
				comp_result = 0;
				return comp_result;
			}
		} else {
			printf("ERROR: bigger in eval.c\n");
			return (-1);
		}
		node = node->cdr;
	}
	return comp_result;
}
int equal ( node_t *node ) {
	int first_number = 0;
	int tmp_number = 0;
	int comp_result = 0;	//1 means True and 0 means False. Tentatively the result is False.
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {//最初の引数をfirst_numberに格納する。
		first_number = eval ( node->car );
		node = node->cdr;
	}
	if ( node->tt == CLOSE ) {
		printf("ERROR:equal in eval.c: You need two or more arguments!\n");
		return (-1);
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );	//2番目の引数からtmp_numberに格納して、比較をはじめる。
			if ( first_number == tmp_number ) {	//比較結果がTrueなら比較するnumberを次にすすめる。(< 1 2 3 ) がTrue. (< 1 4 2)はFalse.
				comp_result = 1;
				first_number = tmp_number;
			}
			else if ( first_number /= tmp_number ) {//比較結果がFalseならFalse ( = 0 )を返す。to do "Distinguish T, Nil between 1, 0"
				comp_result = 0;
				return comp_result;
			}
		} else {
			printf("ERROR: equal in eval.c\n");
			return (-1);
		}	
		node = node->cdr;
	}
	return comp_result;
}
