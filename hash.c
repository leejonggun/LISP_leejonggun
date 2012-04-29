#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

int func_hash (const char *key);
node_t *hash_search(hash_table_t *table, node_t *node);

hash_entry_t *top[HASH_SIZE];
void hash_set(hash_table_t *table, node_t *key, node_t *value) {	//*key is function name. *value is arguments of function.
	int bucket = 0;	//value of func_hash
/*entry に 関数名key と関数の引数リストvalueを代入。*/	
	hash_entry_t *entry = (hash_entry_t*) malloc (sizeof (hash_entry_t));
	entry->key = (const char*) malloc (sizeof (strlen (key->car->character))+1);
	strcpy ((char*)entry->key, key->car->character);	// 関数名を entry の key にコピーする。
	if ( func_call_flag == 1 ) {
		entry->value = copy_node ( value );
	} else {
		entry->value = copy_node ( value );	// ( x y ) (+ x y)引数のリストをコピーする。
	}	
	/*ハッシュ関数にかけて、hast_table(hash_entry[HASH_SIZE])配列の何番目に格納するかを決定する。*/
	bucket = func_hash ( entry->key );	//table の bucket番目のentryに入れる。
		table->entry[bucket] = entry;	//stack.
		table->entry[bucket]->next = top[bucket];
		top[bucket] = table->entry[bucket];
}

/* ハッシュ関数 : key % HASH_SIZE でもいいけど、keyが文字列なので、strlen(key) % HASH_SIZE にしてみる。*/
int func_hash (const char *key) {
	return (strlen (key)) % HASH_SIZE;
}

node_t *hash_search(hash_table_t *table, node_t *node) {	//tableの中のentryに連結されているのを探索する。(keyが違うのにvalueが等しい場合があるのでそれを防止する)
	int bucket = 0;
	hash_entry_t *p;
	bucket = func_hash (node->character);
	//対応するものを見つけたらそのkey(関数名)に対応するvalue(引数)を返す。
	for (p = table->entry[bucket]; p != NULL; p = p->next) {
		if (strcmp (node->character, p->key) == 0){
			return p->value;
		}
	}
	return NULL;	//対応するものが見つからなかったらNULLを返す
}
