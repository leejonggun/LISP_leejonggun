#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

int length = 0;			//The length of string token.
int jindex = 0;			//index
char *sym_data = NULL;
char operater_data = 0;
int num_data = 0;
enum token_type tt;		//token type
node_t* tmp_node = NULL;

enum token_type symbol (const char *string, int last);	//Read string data.
enum token_type number (const char *string, int first);	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
enum token_type operater (const char *string, int index);	//Recognize the operater and put up tt.
node_t* make_node (node_t *node, enum token_type tt, int token_length);

node_t* make_list () {
	node_t *list = (node_t*) malloc (sizeof (node_t) );
	list->tt = OPEN;
	return list;
}
node_t* connect ( const char *input, int index, node_t *node){
	node_t *list = make_list ();
	do {
		index++;	//次のトークンまでindexを進める。
	} while ( input[index] == ' ' );
	jindex = index;

	tmp_node = tokenize ( input, jindex );
	list->car = tmp_node;
	printf("log 1 :list->car, tokenize(input, jinex) '%p' = '%p'\n",list->car, tmp_node);
	do {
		index++;//次のトークンを調べる。
	} while ( input[index] == ' ');
	if ( input[index] != ')') {// CLOSEじゃないならループを続ける。
		tmp_node = connect ( input, jindex, list->cdr );
		list->cdr = tmp_node;
		printf("log 2 :list->cdr, tmp_node '%p' = '%p'\n",list->cdr, tmp_node);
		return list;
	} else if ( input[index] == ')') {//CLOSEならtokenizeに返してあげる
		jindex = index;
		tmp_node = tokenize ( input, jindex );
		list->cdr = tmp_node;
		printf("log 3 :list->cdr->tt, list->cdr = close_node '%d', '%p' = '%p'\n",list->cdr->tt,list->cdr, tmp_node);
		return list;
	}

	//上記以外の場合はありえないけど、一応syntax処理。
	printf("ERROR: in connect.Something wrong.");
	return NULL;
}
node_t* tokenize (const char *input, int tmp) {	
	while ( input[jindex] == ' ') {		//最初の'('を探す。
		jindex++;
	}
	tmp = jindex;	//Read from index.

	node_t *open_node = (node_t*) malloc (sizeof (node_t) );

	if (input[jindex] == '(') {		//'('～')'まで
		open_node->tt = OPEN;
		printf("OPEN:root, root->tt ='%p', '%d'\n",open_node, open_node->tt);
		jindex++;	//OPEN nodeを読んだ後、次の文字から読むためにindexを進める
			open_node->car = tokenize (input, jindex);
			if ( open_node->car == NULL ) {			//return値がNULLの時はエラーの時で、ループを終了させる。
				printf("ERROR:Please input '(' , ')', number or sting\n");
				return NULL;
			}
			open_node->cdr = connect ( input, jindex, open_node->cdr );
			//connectからはinput[jindex] = ')'になって返ってくる。
			do { //次のトークンが終わりがどうかを確認。
				jindex++;
			} while (input[jindex] == ' ');
			if ( input[jindex] == '\0' ) {
				return open_node;
			}
			
			printf("CLOSE:root, root->tt ='%p', '%d'\n",open_node,open_node->tt);
			return open_node;
	} else if ( input[jindex] == ')') {
		//CLOSEが正常に来た場合:OPEN nodeの返す。
			tt = CLOSE;
			open_node = make_node ( open_node, tt ,1);
//			if ( input[jindex+1] == '\0' ) {	//最後のCLOSEなら終わり。	最後のCLOSEの後、' 'を書かないようにする。
				jindex++;
//			}
			//最後のCLOSEの部分もなぜかその前の要素がprintされる。
			printf("CLOSE: open_node = '%p'\n",open_node);
				return open_node;

	} else if (47 < input[jindex] && input[jindex] < 58) {		//'(', ')'以外
		tt = number(input, jindex);
		open_node = make_node (open_node, tt, length);
		printf("NUMBER: open_node, open_node->number = '%p, %d'\n",open_node,open_node->number);
		return open_node;
	} else if ((64 < input[jindex] && input[jindex] < 91) || (96 < input[jindex] && input[jindex] < 123)){
		tt = symbol (input, jindex);
		open_node = make_node (open_node, tt, length);
		free (sym_data);
		printf("SYMBOL: open_node, open_node->character = '%p, %s'\n",open_node, open_node->character);
		return open_node;
	} else if ( input[jindex] == '+' || input[jindex] == '*' || input[jindex] == '-' || input[jindex] == '/' ) {
		tt = operater (input, jindex);
		open_node = make_node (open_node, tt, 1);
		printf("OPERATER: open_node, open_node->character = '%p, %s'\n",open_node, open_node->character);
		return open_node;
	}
	//上記のどれにも当てはまらなかったらエラー。
	printf("ERROR: Please input '(', ')', 'number' or 'string.'\n");
	return NULL;
}

enum token_type symbol(const char *string, int last){	//Read string data.
	int first = last;
	int i = 0, j = 0;	//character for loop
	while((64 < string[last+1] && string[last+1] < 91) || (96 < string[last+1] && string[last+1] < 123)){
					last++;
	}
	jindex = last;
	length = last - first;
	sym_data = (char*)malloc(sizeof(length+1));//文字数+1分メモリーを確保する。
	j = 0;
	for(i = first; i <= last; i++){
	sym_data[j] = string[i];		
	j++;
	}
	sym_data[j] = '\0';
	return SYMBOL;
}

enum token_type number(const char *string, int first){	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
	int last = first;
	int num_token = string[last]-48;	//最初の数字
	while(47 < string[last+1] && string[last+1] < 58){
		num_token = num_token * 10 + (string[last+1]-48);
		last++;
	}
	jindex = last;			//最後の数字のindexを覚える。
	length = last - first;
	num_data = num_token;
	return NUMBER;
}

enum token_type operater(const char *string, int index){	//Recognize the operater and put up tt.
	operater_data = string[index];
	switch(operater_data){
		case '+':
		case '*':
		case '-':
		case '/':
			return OPERATER;
		default:
			operater_data = OPERATER;
			printf("error\n");
			return OPERATER;
	}
}

node_t* make_node (node_t *node, enum token_type tt, int token_length){
	node->tt = tt;
	if (tt == OPERATER) {		//Atom_node
		node->cdr = NULL;
		node->character = &operater_data;//node->character got operater.
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
		node->car = NULL;
		node->cdr = NULL;
		return node;
	} else {
		printf("error make_node\n");
	}
	return NULL;
}
