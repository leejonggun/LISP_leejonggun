#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

int length = 0;			//The length of string token.
int jindex = 0;			//index
char *sym_data = NULL;
char operator_data = 0;
int num_data = 0;
enum token_type tt;		//token type
node_t* tmp_node = NULL;

enum token_type symbol (const char *string, int last);	//Read string data.
enum token_type number (const char *string, int first);	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
enum token_type operator (const char *string, int index);	//Recognize the operator (including Compare operator) and put up tt.
node_t* make_node (node_t *node, enum token_type tt, int token_length);

node_t* make_list () {	//listを作る。
	node_t *list = (node_t*) malloc (sizeof (node_t) );
	list->tt = OPEN;
	return list;
}

node_t* tokenize (const char *input) {	
	while ( input[jindex] == ' ') {		//最初の'('を探す。
		jindex++;
	}
		node_t *open_node = (node_t*) malloc (sizeof (node_t) );
	if (input[jindex] == '(') {		//'('～')'まで
		node_t *root = open_node;
		open_node->tt = OPEN;
//		printf("OPEN:root, root->tt ='%p', '%d'\n",open_node, open_node->tt);
		jindex++;	//OPEN nodeを読んだ後、次の文字から読むためにindexを進める
		while ( input[jindex] != ')') {
			open_node->car = tokenize (input);
			if ( open_node->car == NULL ) {
				return NULL;
			}
			open_node->cdr = make_list ();
			open_node = open_node->cdr;
			while ( input[jindex] == ' ') {
			jindex++;
			}
			//CLOSEの前の' 'を読み飛ばす。
		}
		//CLOSEが正常に来た場合:OPEN nodeの返す。
		tt = CLOSE;
		//open_node->car = NULL;
		//open_node->cdr = NULL;
		open_node = make_node ( open_node, tt, 1);
		jindex++;
//		printf("CLOSE;root, root->tt ='%p', '%d'\n",root, root->tt);
		return root;
		
	} else if ( input[jindex] == ')') {	//CLOSEエラー処理。
			printf("CLOSE: Something Wrong.\n");
				return NULL;

	} else if (47 < input[jindex] && input[jindex] < 58) {		//'(', ')'以外
		tt = number(input, jindex);
		open_node = make_node (open_node, tt, length);
//		printf("NUMBER:open_node->number = '%d'\n",open_node->number);
		return open_node;
	} else if ((64 < input[jindex] && input[jindex] < 91) || (96 < input[jindex] && input[jindex] < 123)){
		tt = symbol (input, jindex);
		open_node = make_node (open_node, tt, length);
		free (sym_data);
//		printf("SYMBOL:open_node->character = '%s'\n", open_node->character);
		return open_node;
	} else if ( input[jindex] == '+' || input[jindex] == '*' || input[jindex] == '-' || input[jindex] == '/' || input[jindex] == '<' || input[jindex] == '>' || input[jindex] == '=' ) {	//to do '<=', '>=', '/='
		tt = operator (input, jindex);
		open_node = make_node (open_node, tt, 1);
//		printf("OPERATOR:open_node->character = '%s'\n", open_node->character);
		return open_node;
	}
	//上記のどれにも当てはまらなかったらエラー。
	printf("ERROR: Please input '(', ')', '+', '*', '-', '/', '<', '>', '=', 'number' or 'string.'\n");
	return NULL;
}

enum token_type symbol(const char *string, int last){	//Read string data.
	int first = last;
	int i = 0, j = 0;	//character for loop
	while((64 < string[last+1] && string[last+1] < 91) || (96 < string[last+1] && string[last+1] < 123)){
					last++;
	}
	jindex = last;
	length = last - first+1;
	sym_data = (char*)malloc(sizeof(length+1));//文字数+1分メモリーを確保する。
	j = 0;
	for(i = first; i <= last; i++){
	sym_data[j] = string[i];		
	j++;
	}
	sym_data[j] = '\0';
	jindex++;
	return SYMBOL;
}

enum token_type number(const char *string, int first){	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
	int last = first;
	int num_token = string[last]-48;	//最初の数字
	while(47 < string[last+1] && string[last+1] < 58){
		num_token = num_token * 10 + (string[last+1]-48);
		last++;
	}
	jindex = last+1;			//最後の数字のindexを覚える。
	length = last - first;
	num_data = num_token;
	return NUMBER;
}

enum token_type operator(const char *string, int index){	//Recognize the operator(including comparison) and put up tt.
	operator_data = string[index];
	switch(operator_data){
		case '+':
		case '*':
		case '-':
		case '/':
			jindex++;
			return OPERATOR;
		case '<':
		case '>':
		case '=':		//to do '<=','>=','/='
			jindex++;
			return COMP;
		default:
			operator_data = OPERATOR;
			printf("error\n");
			jindex++;
			return OPERATOR;
	}
}

node_t* make_node (node_t *node, enum token_type tt, int token_length){
	node->tt = tt;
	if (tt == OPERATOR || tt == COMP) {		//Atom_node
		node->cdr = NULL;
		char *calc = (char *)malloc (2);
		calc[0] = operator_data;
		calc[1] = '\0';
		node->character = calc;
		printf("TODO:make_node in tokenize.c: '<=', '>=', '/='\n");
//		node->character = &operator_data;//node->character got operator.
		return node;	
	} else if (tt == NUMBER) {			//Atom
		node->cdr = NULL;			//Atom has no cdr.
		node->number = num_data;	//copy num_data.
		return node;
	} else if (tt == SYMBOL) {			//Call Atom function
		node->cdr = NULL;
		node->character = (char*)malloc(sizeof(token_length));	//j means sym_data's length.
		int i = 0;
		for (i = 0; i <= token_length; i++) {			// copy sym_data.
		node->character[i] = sym_data[i];
		}
		return node;
	} else if (tt == CLOSE ) {
		//node->tt = CLOSE;
		node->car = NULL;
		node->cdr = NULL;
		return node;
	} else {
		printf("error make_node\n");
	}
	return NULL;
}
