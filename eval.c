#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

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
	printf("IN string_cmp\n");
	for ( i = 0; i < (strlen (node->character)) ; i++ ) {
		printf("log:string_cmp:in eval.c: node->character[%d] = '%c', string[%d] = '%c'\n", i, node->character[i], i, string[i]);
		if ( string[i] != node->character[i]) {
			return 1;
		}
	}
	return 0;
}
	//setq作り
node_t *belong_setq ( node_t *node ) {
	node_t *setq_num;
	//数字を格納した構造体を返して、func_setqに返す。(setq_variable->cdrにつなげる)
	while ( node->tt != CLOSE ) {
		setq_num = (node_t*) malloc ( sizeof (node_t) );
		setq_num->tt = node->car->tt;
		setq_num->number = eval ( node->car );
		setq_num->cdr = NULL;
		node = node->cdr;
	}
	return setq_num;
}
node_t *func_setq ( node_t *node ) {	// ( setq x 1 ) を入力すると, 変数xが入ってるnodeのcdrは数字1が入ってるnodeを指す。
	node_t *setq_variable = (node_t*) malloc ( sizeof (node_t) );
	if ( node->car->tt == SYMBOL ) {	//"setq"の次のトークンが文字列ならsetq_variableを確保して変数を格納する。
		int symbol_length = strlen (node->car->character);
		setq_variable->tt = node->car->tt;
		setq_variable->character = (char*) malloc ( sizeof ( symbol_length+1 ));
		strcpy (setq_variable->character, node->car->character); 
		//setq_variable->character = node->car->character; // eval ( node->car ); 
		//上の意味はsetq_variableのcharacter(setq_variableのchar *characterが宣言されている。)にnode->car->characterを代入(更新)している?setq_variableのポインタcharacterがnode->car->characterを指しているに過ぎない？
		if ( node->cdr->tt == CLOSE ) {// ( setq x ) 引数なしで変数を定義しようとする場合
			printf("ERROR:func_setq in eval.c: SyntaxError. Need more arguments!\n");
			return NULL;
		}
		if ( node->cdr->car->tt == NUMBER ) {
			setq_variable->cdr = belong_setq ( node->cdr );	//正常処理。文字列の次のトークンが数字なら数字を読み込んで、文字列のcdr->数字のアドレスとする。
			return setq_variable;
		} else {
			printf("ERROR:func_setq in eval.c: SyntaxError\n");
			return NULL;
		}
		//else if ( node->cdr->car->tt == SYMBOL ) {		//もし、文字列の次のトークンも文字列ならもう一度func_setqを呼ぶ。to do
		//	setq_variable->cdr = func_setq ( node->cdr );
		//}
//	to do
	} else if ( node->car->tt == NUMBER ) {
		setq_variable->tt = node->car->tt;
		setq_variable->number = eval ( node->car );
		setq_variable->cdr = NULL;
		return setq_variable;
	} else {
		printf("ERROR1:in eval.c.func_setq\n");
		return NULL;
	}
	printf("ERROR2:in eval.c.func_setq\n");
	return NULL;	//これがないとコンパイル時warning出る。
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
			/*比較演算の選択*/
		} else if( node->car->tt == COMP ) {
			if ( node->car->character[0] == '<' ) {
				eval_result = smaller ( node->cdr );// ( < 1 2 ) means True. (1 < 2 is true)
			} else if ( node->car->character[0] == '>' ) {
				eval_result = bigger ( node->cdr );// ( > 1 2 ) means False. (1 > 2 is false)
			} else if ( node->car->character[0] == '=' ) {
				eval_result = equal ( node->cdr );// ( = 1 1 ) means True. (1 = 1 is true)
			}
			return eval_result;
//		else {
			/* to do ERROR */
			/*if文,setq文などの関数が来た場合*/
		} else if (node->car->tt == SYMBOL) {
			int symbol_result = 0;
			if ( string_cmp (node->car, "if" ) == 0 ) {
				symbol_result = func_if (node->cdr);
			/* to do if文作成 */
			} else if ( string_cmp ( node->car, "setq" ) == 0 ) {
				printf("to do setq_func\n");
				node_t *setq_node;
				setq_node = func_setq ( node->cdr );
				if ( setq_node == NULL ) {
					printf("ERROR:func_setq in func_setq. SyntaxError.");
					return (-1);
				}
				printf("setq_node->character, setq_node->cdr->car->number = '%s, %d'\n", setq_node->character, setq_node->cdr->number);
				free (setq_node->character);	//to do 本当はlispを終了するまでfreeしないで持っておく
				free (setq_node);	//to do lispを終了する(quit関数を作る)とfreeする。
				printf("TODO:func_setq in eval.c: Tentatively func_setq is working. Now the variable is freed. But, I have to keep the variable\n");
				return 0;
			}
		return symbol_result;
		}
	/*数字なら数字を返す*/
	} else if (node->tt == NUMBER) {
	
		return  node->number;

	} 
//上記のどれにも該当しないならエラー処理。
	return (-1);
}

int add ( node_t *node ) {
	int tmp_number = 0;
	//	ここでevalを呼ぶ。CLOSEまでwhileでループする。
	int add_result = 0;
	if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
	add_result = eval ( node->car );
	node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
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
	if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
		sub_result = eval ( node->car );
		node= node->cdr;
	}
	while ( node->tt != CLOSE ) {
			if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
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
	if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
		mul_result = eval ( node->car );
		node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
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
	if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
		dir_result = eval ( node->car );
		node = node->cdr;
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
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
	if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {//最初の引数をfirst_numberに格納する。
		first_number = eval ( node->car );
		node = node->cdr;
	}
	if ( node->tt == CLOSE ) {
		printf("ERROR:smaller in eval.c: You need two or more arguments!\n");
		return (-1);
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
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
	if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {//最初の引数をfirst_numberに格納する。
		first_number = eval ( node->car );
		node = node->cdr;
	}
	if ( node->tt == CLOSE ) {
		printf("ERROR:bigger in eval.c: You need two or more arguments!\n");
		return (-1);
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
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
	if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {//最初の引数をfirst_numberに格納する。
		first_number = eval ( node->car );
		node = node->cdr;
	}
	if ( node->tt == CLOSE ) {
		printf("ERROR:equal in eval.c: You need two or more arguments!\n");
		return (-1);
	}
	while ( node->tt != CLOSE ) {
		if ( node->car->tt == NUMBER || node->car->tt == OPEN ) {
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
