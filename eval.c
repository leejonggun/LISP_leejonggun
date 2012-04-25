#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

hash_table_t *setq_table = NULL;
hash_table_t *defun_table = NULL;
hash_table_t *tmp_table = NULL;
int Comp_flag = 0;
int func_call_flag = 0;
//int t = 0;

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
			//	printf("in if: symbol_result ='%d'\n",symbol_result);
				return symbol_result;
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
					defun_table->prev = (hash_table_t*) malloc (sizeof (hash_table_t) );
			//		defun_table->prev = NULL;
				}
				hash_set ( defun_table, node->cdr, node->cdr->cdr );//hash tableのどこかに格納。
				//
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
						node_t *args_given = node->cdr;
						while (args_given->car->tt == OPEN) {
							args_given = args_given->car;
						}
						node_t *function = func_value->cdr->car;
						func_call_flag = 1;
						if ( length_cdr ( func_args ) != length_cdr ( node->cdr ) ) {
							printf("ERROR: Too many of too few arguments, you give.\n");
							return (-1);
						}
						//hash_table_t *newtable = (hash_table_t*) malloc (sizeof(hash_table_t) );
						//newtable->prev = defun_table->prev;
						//defun_table->prev = newtable;
				
						while ( func_args->tt != CLOSE ) {//与えられた引数(args_given)を評価した値を関数の引数にsetqする。
							node_t *tmp_args = ( node_t* ) malloc ( sizeof (node_t));
							tmp_args->tt = NUMBER;
							tmp_args->cdr = NULL;
							tmp_args->number = eval ( args_given );	
						
						//	printf("log: call func: tmp_args->number = '%d'\n", tmp_args->number );
							if (tmp_args->number == 1 || tmp_args->number == 2) {
						//		t++;
						//		printf("fib(1),fib(2)が呼び出された回数。'%d'\n",t);
								return 1;//これはfib(2),fib(1)の場合。
							}
							hash_set (defun_table->prev, func_args, tmp_args/*args_given*/);	//defun_table->prevは関数の引き数用のhash_table.

							free(tmp_args);
							args_given = args_given->cdr;
							func_args = func_args->cdr;
						}
						//	setqし終わったら、新しいtableを作って、引数を積む。stack
						hash_table_t *newtable = (hash_table_t*) malloc (sizeof(hash_table_t) );
						newtable->prev = defun_table->prev;
						tmp_table = defun_table->prev;
						defun_table->prev = newtable;
						//printf("tmp_table -> entry[bucket] -> value -> number = '%d'\n",tmp_table->entry[1]->value->number);
						//call some function
						int result = eval ( function );
						//pop the stack.
						defun_table->prev = newtable->prev;
						tmp_table = defun_table->prev->prev;
						free(newtable);
						//free(newtable->prev);
						//printf("tmp_table -> entry[bucket] -> value -> number = '%d'\n",tmp_table->entry[1]->value->number);
						//printf("after result = '%d'\n",result);
						return result;
					} else {	//defun_tableに対応する関数がなかったら、それは変数。
							if ( tmp_table != NULL ) {
								node_t *args_value = hash_search ( tmp_table, node->car );
								printf("args_value->number='%d'\n", args_value->number);
								int args_number = eval ( args_value );
								return args_number;
							}
						printf("You don't define function1'%s'\n", node->car->character);
						return (-1);
					}
				}
				printf("You don't define function2'%s'\n", node->car->character);
				return (-1);
			}
			printf("Something wrong\n");
			return (-1);
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
						node_t *defun_node = hash_search ( tmp_table, node );
						if ( defun_node == NULL ) {
							printf("You don't define the function3'%s'.\n",node->character);	
								return (-1);
						} else {//tmp_tableに対応する変数があったら処理をする。
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
	printf("ERROR: eval in eval.c(255): You couldn't get any process.\n");
	return 1;
}
int func_if ( node_t *node ) {
	int comp_result = 0;
	int run_number = 0;
	if ( node->car->tt == OPEN ) {
		comp_result = eval ( node->car );	//ここでも、とりあえず'1'='True', '0'='False'
		if ( comp_result == 1 ) {	//条件式がTrue( = 1 )ならば条件式の次の文を実行、表示。(if (< 1 2) 3 4)の3に当たる。
			run_number = eval ( node->cdr->car );
		//	t++;
		} else if ( comp_result == 0 ) {//条件式がFalse( = 0 )ならば条件式の次の次の文を実行,表示。(if (< 2 1) 3 4 )の4に当たる。
			run_number = eval ( node->cdr->cdr->car );
		} else {
			printf("ERROR:func_if in eval.c: Comparison result is not '1'(=True) or '0'(=False).\n");
			return (-1);
		}
//		printf("in func_if: run_number = '%d'\n",run_number);
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
