#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

int length = 0;			//The length of string token.
//int jindex = 0;			//index
enum token_type tt;		//token type

enum token_type symbol(const char *string, int last);	//Read string data.
enum token_type number(const char *string, int first);	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
enum token_type operater(const char *string, int index);	//Recognize the operater and put up tt.
node_t* tokenize(const char *input, int tmp) {	
	jindex = tmp;	//Read from index.
	do{
		if(input[jindex] != ' '){
//Recognition round bracket, Operator
			switch(input[jindex]){
				case '(':			//Start of Tokenize
					printf("before make_node:OPEN\n");
					make_node(OPEN, 1);
					jindex++;
					prev_list = tokenize(input, jindex);
					break;
				case ')':			//End of Tokenize
					if(input[jindex+1] == '\0')
						printf("before make_node:last CLOSE\n");
						make_node(CLOSE, 1);
						jindex++;
						return root;
					if(input[jindex+1] != '\0')
						printf("before make_node:CLOSE\n");
						make_node(CLOSE, 1);	
						prev_list->cdr = open_node->cdr;//open_node is previous list of OPEN node. open_node->cdr indicates next list.
						jindex++;
						return prev_list;
//					break;
				case '+':
				case '*':
				case '-':
				case '/':
					tt = operater(input, jindex);
						printf("before make_node:OPERATER\n");
					make_node(tt, 1);
					break;
				default:
//Recognition Number Calling number function
					if(47 < input[jindex] && input[jindex] < 58){
						tt = number(input, jindex);
						printf("before make_node:NUMBER\n");
						make_node(tt,length);
						break;
					}
//Recognition Charactor Calling symbol function
					if((64 < input[jindex] && input[jindex] < 91) || (96 < input[jindex] && input[jindex] < 123)){
						tt = symbol(input, jindex);
						printf("before make_node:SYMBOL\n");
						make_node(tt,length);
						free(sym_data);
						break;
					}
				}
		}else if(input[jindex] == ' '){
			jindex++;
			}
	}while(input[jindex] != ')');
	return open_node;
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
	jindex = last;			//最後の数字のindexを覚える。
	length = last - first;
	num_data = num_token;
	jindex++;				//次のindexにしてループに返す
	return NUMBER;
}

enum token_type operater(const char *string, int index){	//Recognize the operater and put up tt.
	operater_data = string[index];
	switch(operater_data){
//		case '(':
//			jindex++;	//jindexに次のindexを指すようにする。
//			return OPEN;
//			break;
//		case ')':
//			jindex++;
//			return CLOSE;
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
