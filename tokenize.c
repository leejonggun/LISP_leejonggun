#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

/*仕様(入力された文字列をopenが来たらcloseが来るまでループさせる。それぞれのstring(文字と数字と演算子)をデータとして保持する。)
printfの代わりにglobalデータに格納することにする。文字列は格納する度に新しくmallocする。
enum token_type tt = {open '(', close ')', number, symbol(culculation, charactor)} open,close,symbol = *sym_dataへ number = num_data へ
global変数  char* symbol, int number
関数でデータ型を返してあげる。実際のデータはglobal変数に格納する。
入力した文字列のどこまで読み込んだかも覚えておく。(jindexのこと)
*/

/*
入力があると、トークンごとに分けてデータを保存する。方法はキューに入れる。キューのフィールドは*prev, 共同体token_type, sym_data, num_dataとする。トークンごとに分けてデータに保存したらそのデータを順番に呼び出して解釈する(関数を使用)。
*/

enum token_type {OPEN, CLOSE, NUMBER, SYMBOL};	//データ型
enum token_type symbol(const char *string, int last);	//symbol関数は読み込んだ文字列を、mallocで格納して、sym_dataに格納する
enum token_type number(const char *string, int first);	//number関数は読み込んだ数字の最初を渡して、num_dataへ格納する
enum token_type one_symbol(const char *string, int index);	//one_symbol関数はOPEN,CLOSE,OPERATOR専用で文字をone_symbol_dataへ格納する。
char *sym_data = NULL;	//実際の文字列のデータ
int num_data = 0;		//実際の数字のデータ
char  one_symbol_data= 0;
int i = 0, j = 0;	//ループ用変数
int jindex = 0;	//入力された文字数
enum token_type tt;		//タイプ格納

void tokenize(const char *input) {	//void型はreturn不要
	int inputSize = strlen(input);
	int op = 0;	//op means the kind of operator.
	int tmp = 0;		//文字の初めのjindex
	int char_token = 0;	//文字数
	for(jindex = 0; jindex < inputSize; jindex++){
//	do{
		if(input[jindex] != ' '){
//Recognition round bracket, Operator
/*			switch(input[jindex]){
				case '(':
				case ')':
				case '+':
				case '*':
				case '-':
				case '/':
					tt = one_symbol(input, jindex);
					printf("one_symbol_data '%C', tt = %d\n",one_symbol_data,tt);
					break;
				default:
					*/		
			if(input[jindex] == '('){
				tt = one_symbol(input, jindex);	//ttの値で条件分けする。ttの値がOPENなら他の関数を呼んで、CLOSEなら文字の読み込み、データに格納を終了する。SYMBOLならその時のinput[op]の値により演算子の区別ができる。
				printf("one_symbol_data'%c', tt = %d\n",one_symbol_data,tt);
			}
			if(input[jindex] == ')'){
				tt = one_symbol(input, jindex);
				printf("one_symbol_data'%c', tt = %d\n",one_symbol_data,tt);
			}
			if(input[jindex] == '*'){
				tt = one_symbol(input, jindex);
				printf("one_symbol_data'%c', tt = %d\n",one_symbol_data,tt);
			}else if(input[jindex] == '+'){
				tt = one_symbol(input, jindex);
				printf("one_symbol_data'%c', tt = %d\n",one_symbol_data,tt);
			}else if(input[jindex] == '-'){
				tt = one_symbol(input, jindex);
				printf("one_symbol_data'%c', tt = %d\n",one_symbol_data,tt);
			}else if(input[jindex] == '/'){
				tt = one_symbol(input, jindex);
				printf("one_symbol_data'%c', tt = %d\n",one_symbol_data,tt);
			}

//Recognition Number 関数numberを呼び出す。
			if(47 < input[jindex] && input[jindex] < 58){
					tt = number(input, jindex);
printf("log_number num_data = %d, tt = %d\n",num_data,tt);
				continue;
//				break;
			}
//Recognition Charactor 関数symbolを呼び出す。
			if(64 < input[jindex] && input[jindex] < 91 || 96 < input[jindex] && input[jindex] < 123){
				tt = symbol(input, jindex);
printf("log_symbol sym_data = %s, tt = %d\n",sym_data,tt);
				free(sym_data);
				continue;
//				break;
			}
		
		}else if(input[jindex] == ' '){
			printf("");
//			jindex++;
		}
	}//while(jindex != inputSize);
}

enum token_type symbol(const char *string, int last){
	int first = last;
	while(64 < string[last+1] && string[last+1] < 91 || 96 < string[last+1] && string[last+1] < 123){
					last++;
	}
	jindex = last;
	int length = last - first;
//	char *buf;
//	buf = (char*)malloc(sizeof(length+1));		//文字数+1分メモリーを確保する。
	sym_data = (char*)malloc(sizeof(length+1));
//*buf1 = (char*)malloc(sizeof(char_token+1));じゃないところがわからない。buf1はメモリー配列の最初のアドレス。*buf1はbuf1の指すアドレスの中身
	j = 0;
	for(i = first; i <= last; i++){
//	buf[j] = string[i];							//どうして *buf1[j] = input[i]; じゃだめか？
	sym_data[j] = string[i];		
	j++;
	}
//	buf[j] = '\0';								//文字列の最後は\0です。
	sym_data[j] = '\0';
//	printf("log_symbol sym_data = %s\n", sym_data);

//	sym_data = buf;					//文法の間違い。bufの文字列をsym_dataにコピーしたことにならない
//	free(buf);						//メモリーの開放。
	return SYMBOL;
}

enum token_type number(const char *string, int first){	//number関数は読み込んだ数字の最初を渡して、enum ttのnumberへ格納する
	int num_token = string[first]-48;	//数字のトークン
	enum token_type tt;
	while(47 < string[first+1] && string[first+1] < 58){
		num_token = num_token * 10 + (string[first+1]-48);
		first++;
	}
	jindex = first;			//最後の数字のindexを覚える。
	num_data = num_token;
	return NUMBER;
}

enum token_type one_symbol(const char *string, int index){
	one_symbol_data = string[index];
	switch(one_symbol_data){
		case '(':
//			jindex++;	//jindexに次のindexを指すようにする。
			return OPEN;//文字を続けて読み込む処理を加える。
			break;
		case ')':
//			jindex++;
			return CLOSE;//文字の読み込みを終了する処理を加える。
			break;
		case '+':
		case '*':
		case '-':
		case '/':
//			jindex++;
			return SYMBOL;//文字を続けて読み込む処理を加える。
			break;
		default:
			one_symbol_data = 0;
//			jindex++;
	printf("error one_symbol_data = %d, token_type = %d\n",one_symbol_data,tt);
			return SYMBOL;
			break;
	}
}
