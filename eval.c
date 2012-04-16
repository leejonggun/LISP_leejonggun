#include <stdio.h>
#include "lisp.h"

int add ( node_t *node );
int sub ( node_t *node );
int mul ( node_t *node );
int dir ( node_t *node );

int eval ( node_t *node ) {
	int eval_result = 0;
//	char *operator = NULL;
//	int list_num = 0;
//	int eval_number = 0;
//	char *symbol = NULL;
//	node_t *node_car;
//	node_t *node_cdr;
	if ( node->tt == OPEN ) {
		if ( node->car->tt == OPERATOR ) {
			/* 関数を見つける */
			/*四則演算の選択*/
			if ( node->car->character[0] == '+' ) {
				eval_result = add ( node->cdr );
			} else if ( node->car->character[0] == '-' ) {
				eval_result = sub ( node->cdr );
			} else if ( node->car->character[0] == '*' ) {
				eval_result = mul ( node->cdr );
			} else if ( node->car->character[0] == '/' ) {
				eval_result = dir ( node->cdr );
			}
//		else if (node->car->tt == SYMBOL) {
		/* to do */
//		}
//		else {
			/* ERROR */
	}
		return eval_result;
	} else if (node->tt == NUMBER) {
	
		return  node->number;

//	} else if (node->tt == SYMBOL) {
//		symbol = node->character;
	}
//上記のどれにも該当しないならエラー処理。
	return (-1);
}

int add ( node_t *node ) {
	int tmp_number = 0;
	//	ここでevalを呼ぶ。CLOSEまでwhileでループする。
	int add_result = 0;
	if ( node->car->tt == NUMBER ) {
	add_result = node->car->number;
	node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: in eval.c.add\n");
			return (-1);
		}
		add_result += tmp_number;	//add_result = add_result + tmp_number;
		node = node->cdr;
	}
	return add_result;
}
int sub ( node_t *node ) {
	int tmp_number = 0;
	int sub_result = 0;
	if ( node->car->tt == NUMBER ) {
	sub_result = node->car->number;
	node = node->cdr;
	}
		while ( node->tt != CLOSE ) {
			if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: in eval.c.add\n");
			return (-1);
		}
		sub_result -=  tmp_number;	//sub_result = sub_result - tmp_number;
		node = node->cdr;
	}
	return sub_result;	
}
int mul ( node_t *node ) {
	int tmp_number = 0;
	int mul_result = 0;
	if ( node->car->tt == NUMBER ) {
		mul_result = node->car->number;
		node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: in eval.c.add\n");
			return (-1);
		}
		mul_result *= tmp_number;	//mul_result = mul_result * tmp_number;
		node = node->cdr;
	}
	return mul_result;
}
int dir ( node_t *node ) {
	int tmp_number = 0;
	int dir_result = 0;
	if ( node->car->tt == NUMBER ) {
		dir_result = node->car->number;
		node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: in eval.c.add\n");
			return (-1);
		}
		dir_result /= tmp_number;	//dir_result = sub_result / tmp_number;
		node = node->cdr;
	}
	return dir_result;
}

