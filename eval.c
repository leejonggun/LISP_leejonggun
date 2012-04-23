#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

hash_table_t *setq_table = NULL;
hash_table_t *defun_table = NULL;
int Comp_flag = 0;
int func_call_flag = 0;

int add ( node_t *node );
int sub ( node_t *node );
int mul ( node_t *node );
int dir ( node_t *node );
int smaller ( node_t *node );
int bigger ( node_t *node );
int equal ( node_t *node );
int func_if ( node_t *node);
int string_cmp (node_t *node, char *string) {	//文字列の比較
	int i;
	for ( i = 0; i < (strlen (node->character)) ; i++ ) {
		if ( string[i] != node->character[i]) {
			return 1;
		}
	}
	return 0;
}
int length_cdr (node_t *first_OpenNode) {	//トークンのcdrの数を調べる。
	int length = 0;
	while ( first_OpenNode->tt != CLOSE ) {
		length++;
		first_OpenNode = first_OpenNode->cdr;
	}
	return length;
}
int eval ( node_t *node ) {
	int eval_result = 0;
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
			return eval_result;
			/*比較演算の選択*/ //to do フラグをつける。計算結果との区別
		} else if( node->car->tt == COMP ) {
			Comp_flag = 1;	
			if ( node->car->character[0] == '<' ) {
				eval_result = smaller ( node->cdr );// ( < 1 2 ) means True. (1 < 2 is true)
			} else if ( node->car->character[0] == '>' ) {
				eval_result = bigger ( node->cdr );// ( > 1 2 ) means False. (1 > 2 is false)
			} else if ( node->car->character[0] == '=' ) {
				eval_result = equal ( node->cdr );// ( = 1 1 ) means True. (1 = 1 is true)
			}
			return eval_result;
		} else if (node->car->tt == NUMBER) {
			return node->car->number;
		} else if (node->car->tt == OPEN) {
			return eval (node->car);
		}else if (node->car->tt == SYMBOL) {
			int symbol_result = 0;
			/* if文 */
			if ( string_cmp (node->car, "if" ) == 0 ) {
				if ( length_cdr ( node ) != 4 ) {
					printf("ERROR:func_if in eval.c: SyntaxError.( if (x) arg1 arg2 )\n");
					return (-1);
				}
				symbol_result = func_if (node->cdr);
			/*setq文*/
			} else if ( string_cmp ( node->car, "setq" ) == 0 ) {
				if ( length_cdr ( node ) != 3 ) {
					printf("ERROR:setq in eval.c: SyntaxError.( setq x arg1 )\n");
					return (-1);
				}
				if ( setq_table == NULL ) {//最初に呼ばれたときだけmallocする。
					setq_table = (hash_table_t*) malloc (sizeof (hash_table_t) );
				}
				//node_t *search_result = hash_search (setq_table, node->cdr);
				//対応するものがtableにないなら格納可能。
				//対応するものがtableにある。table->entry[bucket]->nextにpushする。
				node_t *tmp_setq = (node_t*) malloc (sizeof (node_t));//setqのvalueを計算しておく
				tmp_setq->number = eval ( node->cdr->cdr->car );
				tmp_setq->tt = NUMBER;
				tmp_setq->cdr = NULL;
				hash_set ( setq_table, node->cdr, tmp_setq );//hash tableのどこかに格納。
				int tmp_num = tmp_setq->number;
				free(tmp_setq);
				printf("log: SUCCESS to set setq_hash_table.\n");
				return tmp_num;
			/*defun文*/
			} else if (string_cmp ( node->car, "defun") == 0 ) {
				if ( length_cdr ( node ) != 4 ) {
					printf("ERROR:defun in eval.c: SyntaxError.( defun name (args) (expression)\n");
					return (-1);
				}
				if ( defun_table == NULL ) {
					defun_table = (hash_table_t*) malloc (sizeof (hash_table_t) );
				}
				//node_t *search_result = hash_search (defun_table, node->cdr);
				//if ( search_result == NULL ) {//対応するものがtableにないなら格納可能。
				//	hash_set ( defun_table, node->cdr, node->cdr->cdr );
				//} else if ( search_result != NULL ) {//対応するものがtableにある。table->entry[bucket]->nextにpushする。
				hash_set ( defun_table, node->cdr, node->cdr->cdr );//hash tableのどこかに格納。
				//}
				printf("log: SUCCESS to set defun_hash_table\n");
//				printf("defun_table->entry[1]->key'%s'\n",defun_table->entry[1]->key);
//				printf("defun_table->entry[1]->value->car->car->character'%s'\n",defun_table->entry[1]->value->car->car->character);
				return symbol_result;
			/* 関数呼び出し。( f 2 3 )が渡されたとき。*/
			} else {
				if (/* setq_table != NULL ||*/ defun_table != NULL ) {
					node_t *func_value = hash_search(defun_table, node->car);//defun_table中に関数名(node->car->character)があるかどうか判断。
					if ( func_value != NULL ) {//defun_tableに対応する関数があったら、
						node_t *func_args = func_value->car;//func_argsは defun f ( n m ) ( + n m )の最初のOPENを指す。二番目のOPENはfunc_value->cdr->car.
						defun_table->prev = (hash_table_t*) malloc (sizeof(hash_table_t) );//関数の引き数用のhash table。
						node_t *args_given = node->cdr;
						if ( length_cdr ( func_args ) != length_cdr ( node->cdr ) ) {
							printf("ERROR: Too many of too few arguments, you give.\n");
							return (-1);
						}
						func_call_flag = 1;
						while ( func_args->tt != CLOSE ) {//与えられた引数を関数の引数にsetqする。
				//			node_t *tmp_args = ( node_t* ) malloc ( sizeof (node_t));
				//			tmp_args->tt = NUMBER;
				//			tmp_args->cdr = NULL;
				//			tmp_args->number = eval ( args_given );
							hash_set (defun_table->prev, func_args, /*tmp_args*/ args_given);	//defun_table->prevは関数の引き数用のhash_table.
			//				free(tmp_args);
							args_given = args_given->cdr;
							func_args = func_args->cdr;
						}//setqし終わったら、その文字を使ってevalに渡す。計算してもらう。
						node_t *function = func_value->cdr->car;
						printf("before\n");
						int result = eval ( function );
						printf("after result = '%d'\n",result);
						return result;//eval ( function );
					} else {
						printf("You don't define function1'%s'\n", node->car->character);
						return (-1);
					}
				}
				printf("You don't define function2'%s'\n", node->car->character);
				return (-1);
			}
			/*引数用のsetq_tableにpush.*/
			// else 				if ( node->car == hash_search ( defun_table, node->car)) 
			//		printf("log:asdf.\n");
					/*定義した関数名がsetq_table中に見つかった*/
			//		return eval (hash_search (setq_table, node->car));
			// else if ( node->car == hash_search ( defun_table, node->car)) 
					/*定義した変数名がdefun_table中に見つかった*/
					//node_t *defun_node = hash_search ( defun_table, node->car );
					
				//	node->carには関数の引数を渡す。(f 2 3 )の(2 3)にあたる。
			//		return eval (hash_search (defun_table, node->car));
			//	
		}
	/*数字なら数字を返す*/
	} else if (node->tt == NUMBER) {
		return  node->number;
	/*文字が来たらhash_tableの中を検索。*/
	} else if (node->tt == SYMBOL) {
		if ( setq_table != NULL || defun_table != NULL) {
			//引数検査。引数をsetq_tableにsetq。
			node_t *setq_node = NULL;
			if ( setq_table != NULL ){
				setq_node = hash_search ( setq_table, node );
			}
			if (setq_node == NULL) {
				if ( defun_table != NULL ) {
					if (defun_table->prev != NULL) {
						node_t *defun_node = hash_search ( defun_table->prev, node );
						if ( defun_node == NULL ) {
							printf("You don't define the function3'%s'.\n",node->character);	
								return (-1);
						} else {//defun_tableに対応するものがあったら処理をする。
							int defun_result = eval ( defun_node );
							return defun_result;
						}
					} else {
					printf("You don't define the variable'%s'.\n",node->character);
					return (-1);
					}
				}
			} else {
				return eval ( setq_node );
			}
		}
		printf("ERROR:eval in eval.c: SyntaxError. You don't define the character'%s'\n",node->character);
		return (-1);
	}
//上記のどれにも該当しないならエラー処理。
	printf("ERROR: eval in eval.c(188): You couldn't get any process.\n");
	return (-1);
}
int add ( node_t *node ) {
	int tmp_number = 0;
	//	ここでevalを呼ぶ。CLOSEまでwhileでループする。
	int add_result = 0;
	if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
	add_result = eval ( node->car );
	node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN || node->car->tt == SYMBOL) {
			tmp_number = eval ( node->car );
		} else {
			printf("ERROR: add in eval.c\n");
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
		sub_result -=  tmp_number;	//sub_result = sub_result - tmp_number;
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
		mul_result *= tmp_number;	//mul_result = mul_result * tmp_number;
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
		dir_result /= tmp_number;	//dir_result = sub_result / tmp_number;
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
int func_if ( node_t *node ) {
	int comp_result = 0;
	int run_number = 0;
	if ( node->car->tt == OPEN ) {
		comp_result = eval ( node->car );	//ここでも、とりあえず'1'='True', '0'='False'
		if ( comp_result == 1 ) {	//条件式がTrue( = 1 )ならば条件式の次の文を実行、表示。(if (< 1 2) 3 4)の3に当たる。
			run_number = eval ( node->cdr->car );
		} else if ( comp_result == 0 ) {//条件式がFalse( = 0 )ならば条件式の次の次の文を実行,表示。(if (< 2 1) 3 4 )の4に当たる。
			run_number = eval ( node->cdr->cdr->car );
		} else {
			printf("ERROR:func_if in eval.c: Comparison result is not '1'(=True) or '0'(=False).\n");
			return (-1);
		}
		return run_number;
	} else if ( node->car->character[0] == 't' || node->car->character[0] == 'T' || node->car->number == 1) { //条件式がなく、真( = true )が渡された場合
		comp_result = 1;
		run_number = eval ( node->cdr->car );
	} else if ( string_cmp (node->car, "Nil") == 0 || node->car->number == 0) { //条件式がなく、偽( = false )が渡された場合
		comp_result = 0;
		run_number = eval ( node->cdr->cdr->car );
	} else if (node->cdr->cdr->cdr->tt != CLOSE) {	//この書き方は間違ってないか? この場合は文法を守ってないとき表示。
		printf("ERROR:func_if in eval.c:\n Please write in this format (if (condition) argument1 argument2)\n");
		return (-1);
	} else {
		printf("ERROR:func_if in eval.c: Something wrong.\n");
		return (-1);
	}
	return run_number;
}
