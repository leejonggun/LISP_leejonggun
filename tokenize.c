#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

/*
入力があると、トークンごとに分けてデータを保存する。木構造作る。フィールドは*prev, 共同体token_type, sym_data, num_dataとする。トークンごとに分けてデータに保存したらそのデータを順番に呼び出して解釈する(関数を使用)。
*/
enum token_type {OPEN, CLOSE, NUMBER, SYMBOL, OPERATER};	//データ型
enum token_type symbol(const char *string, int last);	//symbol関数は読み込んだ文字列を、mallocで格納して、sym_dataに格納する
enum token_type number(const char *string, int first);	//number関数は読み込んだ数字の最初を渡して、num_dataへ格納する
enum token_type operater(const char *string, int index);	//one_symbol関数はOPEN,CLOSE,OPERATOR専用で文字をone_symbol_dataへ格納する。
struct node_t* make_node(enum token_type tt);
struct node_t* make_list();

char *sym_data = NULL;	//実際の文字列のデータ
int num_data = 0;		//実際の数字のデータ
char operater_data = 0;	//演算子のデータ
int i = 0, j = 0;	//character for loop
int jindex = 0;	//index
enum token_type tt;		//token type

typedef struct node_t{			//Make tree construction
	enum token_type tt;		//The type of token	
	union{	
			int number;			//The data of number
			char *character;	//The data of string
			struct node_t *car;	//To Atom. Atom has Data. List has the pointer to Atom.
	};
	struct node_t *cdr;			//To List. Atom has NULL. Only has List *cdr.
} node_t;

node_t *open_t = NULL;			//indicates OPEN node.
//node_t *result = NULL;
int tokenize(const char *input, int tmp) {	
	jindex = tmp;	//Read from index.
	do{
		if(input[jindex] != ' '){
//Recognition round bracket, Operator
			switch(input[jindex]){
				case '(':			//Start of Tokenize
//					result = make_node(OPEN);
					make_node(OPEN);
					printf("log open_t = '%p'\n",open_t);
					jindex++;
					jindex = tokenize(input, jindex);
					break;
				case ')':			//End of Tokenize
//					result = make_node(CLOSE);
					make_node(CLOSE);
					jindex++;
					return jindex;
				case '+':
				case '*':
				case '-':
				case '/':
					tt = operater(input, jindex);
//					result = make_node(tt);
					make_node(tt);
					break;
				default:
//Recognition Number Calling number function
					if(47 < input[jindex] && input[jindex] < 58){
						tt = number(input, jindex);
//						result = make_node(tt);
						make_node(tt);
						printf("result->number ='%d',result = '%p'\n",result->number,result);
						break;
					}
//Recognition Charactor Calling symbol function
					if((64 < input[jindex] && input[jindex] < 91) || (96 < input[jindex] && input[jindex] < 123)){
						tt = symbol(input, jindex);
//						result = make_node(tt);
						make_node(tt);
						printf("sym_data='%s' ,result->cdr= '%p',result->character='%s', result ='%p'\n",sym_data,result->cdr,result->character,result);
						free(sym_data);
//						free(node->character);
						break;
					}
				}
		}else if(input[jindex] == ' '){
			jindex++;
			}
	}while(input[jindex] != ')');
	return jindex;
}


enum token_type symbol(const char *string, int last){
	int first = last;
	while((64 < string[last+1] && string[last+1] < 91) || (96 < string[last+1] && string[last+1] < 123)){
					last++;
	}
	jindex = last;
	int length = last - first;
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
	int num_token = string[first]-48;	//最初の数字
	while(47 < string[first+1] && string[first+1] < 58){
		num_token = num_token * 10 + (string[first+1]-48);
		first++;
	}
	jindex = first;			//最後の数字のindexを覚える。
	num_data = num_token;
	jindex++;				//次のindexにしてループに返す
	return NUMBER;
}

enum token_type operater(const char *string, int index){
	operater_data = string[index];
	switch(operater_data){
//		case '(':
//			jindex++;	//jindexに次のindexを指すようにする。
//			return OPEN;
//			break;
		case ')':
			jindex++;
			return CLOSE;
		case '+':
		case '*':
		case '-':
		case '/':
			jindex++;
			return OPERATER;
		default:
			operater_data = 0;
			jindex++;
			printf("error");
			return OPERATER;
	}
}

void make_node(enum token_type tt){
	node_t *node = (node_t*)malloc(sizeof(node_t));
	node_t *list;			//for List_node
	node->tt = tt;

	if(tt == OPEN){
		open_t = node;		//tree construction starts from open_t, '('.*open_t is global pointer for '(' token
	
	}else if(tt == CLOSE){
		free(node);
		open_t->cdr = NULL;	//The last Atom node has no cdr and car.
		open_t->car = NULL;
		printf("CLOSE:open_t->cdr='%p'\n",open_t->cdr);
	
	}else if(tt == OPERATER){		//Atom_node
		node->cdr = NULL;
		node->character = &operater_data;//node->character got operater.
		printf("*node->character='%c', operater_data ='%c'",*node->character,operater_data);
// What should be in *car?
		open_t->car = node;			//open_t has the address of '(' node. The Atom_node's address be into open_t->car.
		list = make_list();			//make list_node for next token.
		open_t->cdr = list;			//	connect '(' token to list_node.
		printf("open_t->car,node='%p, %p', open_t->cdr, list='%p, %p',*open_t->car->character ='%c'\n",open_t->car,node,open_t->cdr,*open_t->car->character);
		open_t = list;
	
	}else if(tt == NUMBER){			//Atom
		node->cdr = NULL;			//Atom has no cdr.
		node->number = num_data;	//copy num_data.
		open_t->car = node;			//list_node connect to Atom_node
		list = make_list();
		open_t->cdr = list;			//connect previous list_node to new list_node.
		printf("open_t->car,node='%p, %p', open_t->cdr,list ='%p, %p', open_t->car->number='%d'\n",open_t->car,node,open_t->cdr,list,open_t->car->number);
		open_t = list;
	
	}else if(tt == SYMBOL){			//Call Atom function
		node->cdr = NULL;
		node->character = (char*)malloc(sizeof(j));	//j means sym_data's length.
		for(i = 0; i <= j; i++){					// copy sym_data.
		node->character[i] = sym_data[i];
		}
		open_t->car = node;
		list = make_list();
		open_t->cdr = list;
		printf("open_t->car,node='%p, %p', open_t->cdr,list ='%p, %p', open_t->car->character='%s'\n",open_t->car,node,open_t->cdr,list,open_t->car->character);
		open_t = list;
	}else{
		printf("error make_node");
//		return NULL;
	}

//	return node;
}
node_t* make_list(){
	node_t *list = (node_t*)malloc(sizeof(node_t));
	return list;
}

