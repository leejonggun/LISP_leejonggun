#include <stdio.h>
#include "lisp.h"
/*
#define HASH_SIZE 8
typedef struct hash_entry_t {
	    const char *key;
		    node_t *value;
			    // list 
			struct hash_entry_t *next;
} hash_entry_t;
typedef struct hash_table_t {
	    hash_entry_t* entry[HASH_SIZE];
		    // stack 
		    struct hash_table_t *prev;
}hash_table_t;
*/
/* defun f(n) (f(- n 1)) , (f 3)*/
/* func: key=>f, value=>(f (n) f(- n 1)) */
/* args: key=>n, value=>3 */
void hash_set(hash_table_t *table, node_t *key, node_t *value) {	//*key is function name. *value is arguments of function. *table means ...?
	int bucket = 0;	//value of func_hash
	node_t *func_name = key;	//func_name->tt = SYMBOL. args key = node->cdr
	node_t *func_args = value;	//func_args->tt = OPEN. args value = node->cdr->cdr
/*entry に 関数名key と関数の引数リストvalueを代入。*/	
	hash_entry_t *entry;
	entry->key = func_name->car->character;	// 関数名を entry の key に渡す。
	entry->value = func_args->car;	// ( x y z ) 引数のリストの'('アドレスを渡す。
/*ハッシュ関数にかけて、hast_table(hash_entry[HASH_SIZE])配列の何番目に格納するかを決定する。*/
	bucket = func_hash ( entry->key );	//table の bucket番目のentryに入れる。
	table->entry[bucket] = entry;
}

/* ハッシュ関数 : key % HASH_SIZE でもいいけど、keyが文字列なので、strlen(key) % HASH_SIZE にしてみる。*/
int func_hash (const char *key) {
	return strlen (key) % HASH_SIZE;
}

node_t *hash_search(hash_table_t *table, node_t *node) {
	int bucket = 0;
	bucket = func_hash (node->car->character);
	
}
/*
defunが呼ばれた時	hash_set( ~ , ~ , ~ );
関数テーブルに、関数名をkeyに、defun関数の引数全体をvalueとしてセットする

defunした関数を呼び出す時
関数テーブルからvalueを取ってくる
関数の引数用のハッシュテーブルを新たに作って、スタックにプッシュする
関数名をkeyに、関数テーブルから関数本体を取り出す
(引数が正しいか確認する)
	引数用のハッシュテーブルに、値をセットする
	関数終了時に、引数用のテーブルをfreeし、スタックからポップする

	＊関数のテーブルはスタックにしない(一意)
	＊引数はスタック上に保存したいので、引数のテーブルはスタックにする(prevのフィールドを利用する)
*/
