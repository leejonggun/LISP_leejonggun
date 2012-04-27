#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

static int length = 0;			//The length of string token.
int jindex = 0;			//index
char *sym_data = NULL;
char operator_data = 0;
int num_data = 0;
enum token_type tt;		//token type
node_t *tmp_node = NULL;//消すと遅くなる!信じられない!

static enum token_type symbol (const char *string, int last);	//Read string data.
static enum token_type number (const char *string, int last);	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
static enum token_type operator (const char *string, int index);	//Recognize the operator (including Compare operator) and put up tt.
static node_t* make_node (node_t *node, enum token_type tt);

static node_t* make_list () {	//listを作る。
	node_t *list = (node_t*) malloc (sizeof (node_t) );
	list->tt = OPEN;
	return list;
}

node_t* tokenize (const char *input) {	
	while ( input[jindex] == ' ' || input[jindex] == '\n' || input[jindex] == '\t') {
		jindex++;
	}
		node_t *open_node = (node_t*) malloc (sizeof (node_t) );
	if (input[jindex] == '(') {		//'('～')'まで
		node_t *root = open_node;
		open_node->tt = OPEN;
		jindex++;	//OPEN nodeを読んだ後、次の文字から読むためにindexを進める
		while ( input[jindex] != ')') {
			open_node->car = tokenize (input);
			if ( open_node->car == NULL ) {
				printf("You need more token.\n");
				return NULL;
			}
			open_node->cdr = make_list ();//*cdrに新しいnodeをつなげる
			open_node = open_node->cdr;//*cdrをopen_nodeにして、*carに次のトークンをつなげる。
			while (input[jindex] == ' ' || input[jindex] == '\n' || input[jindex] == '\t') {
			jindex++;
			}
			//CLOSEの前の' 'を読み飛ばす。

		}
		//CLOSEが正常に来た場合:OPEN nodeの返す。
		tt = CLOSE;
		open_node = make_node ( open_node, tt);
		jindex++;
		return root;
		
	} else if ( input[jindex] == ')') {	//ここにCLOSEが来た場合、エラー処理。
		free (open_node);
		printf("CLOSE: Something Wrong.\n");
		return NULL;
	} else if (47 < input[jindex] && input[jindex] < 58) {
		tt = number(input, jindex);
		open_node = make_node (open_node, tt);
		return open_node;
	} else if ((64 < input[jindex] && input[jindex] < 91) || (96 < input[jindex] && input[jindex] < 123)){
		tt = symbol (input, jindex);
		open_node = make_node (open_node, tt);
		free (sym_data);
		return open_node;
	} else if ( input[jindex] == '+' || input[jindex] == '*' || input[jindex] == '-' || input[jindex] == '/' || input[jindex] == '<' || input[jindex] == '>' || input[jindex] == '=' ) {	//to do '<=', '>=', '/='
		tt = operator (input, jindex);
		open_node = make_node (open_node, tt);
		return open_node;
	}
	//上記のどれにも当てはまらなかったらエラー。
	printf("ERROR: Please input '(', ')', '+', '*', '-', '/', '<', '>', '=', 'number' or 'string.'\n");
	return NULL;
}

static enum token_type symbol(const char *string, int last) {	//Read string data.
	int first = last;
	int i = 0, j = 0;	//character for loop
	while ( (64 < string[last+1] && string[last+1] < 91) || (96 < string[last+1] && string[last+1] < 123) ) {
					last++;
	}
	jindex = last;
	length = last - first + 1;
	sym_data = (char*) malloc (length + 1);//文字数+1分メモリーを確保する。
	j = 0;
	for(i = first; i <= last; i++) {
	sym_data[j] = string[i];		
	j++;
	}
	sym_data[j] = '\0';
	jindex++;
	return SYMBOL;
}

static enum token_type number(const char *string, int last) {	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
	int first = last;
	int num_token = string[last] - 48;	//最初の数字
	while(47 < string[last + 1] && string[last + 1] < 58) {
		num_token = num_token * 10 + (string[last+1] - 48);
		last++;
	}
	jindex = last + 1;			//最後の数字の次のindexを覚える。
	length = last - first;
	num_data = num_token;
	return NUMBER;
}

static enum token_type operator(const char *string, int index) {	//Recognize the operator(including comparison) and put up tt.
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

static node_t* make_node (node_t *node, enum token_type tt) {
	node->tt = tt;
	if (tt == OPERATOR || tt == COMP) {		//Atom_node
		node->cdr = NULL;
		char *calc = (char *)malloc (2);
		calc[0] = operator_data;
		calc[1] = '\0';
		node->character = calc;
		//printf("TODO:make_node in tokenize.c: '<=', '>=', '/='\n");
//		node->character = &operator_data;//node->character got operator.
		return node;	
	} else if (tt == NUMBER) {			//Atom
		node->cdr = NULL;			//Atom has no cdr.
		node->number = num_data;	//copy num_data.
		return node;
	} else if (tt == SYMBOL) {			//Call Atom function
		node->cdr = NULL;
		node->character = (char*)malloc(sizeof(length));
		int i;
		for (i =0; i <= length; i++) {
			node->character[i] = sym_data[i];
		}
		//strcpy (node->character, sym_data);
		return node;
	} else if (tt == CLOSE ) {
		node->car = NULL;
		node->cdr = NULL;
		return node;
	} else {
		printf("error make_node\n");
	}
	return NULL;
}
