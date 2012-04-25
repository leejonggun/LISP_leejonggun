#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "lisp.h"

typedef struct string_buffer_t {
	size_t size;//現在のサイズ
	size_t capacity;//サイズの最大値
	char *str;//文字列の一文字分
} string_buffer_t;

string_buffer_t *new_string_buffer() {//新しいstringを作る。new_string_bufferをmallocする。strもmallocする。sizeの初期値は0。
	string_buffer_t *new_sb = (string_buffer_t*) malloc ( sizeof (string_buffer_t) );
	new_sb->size = 0;
	new_sb->capacity = 64;
	//new_sb->str = (char*) malloc ( sizeof (new_sb->capacity) );
	new_sb->str = (char*) malloc ( new_sb->capacity );
	return new_sb;
}
void string_buffer_putc(string_buffer_t *sb, char c) {//一文字読み込む。
//	if ( c == '\n' ) {
//		sb->str[sb->size] = '\0';
//		sb->size++;
//	} else {
	if (sb->capacity == sb->size) {
		size_t newcapacity = sb->capacity * 2;
		char *newstr = (char*) malloc ( newcapacity );
		memcpy( newstr, sb->str, sb->capacity );
		free(sb->str);
		sb->str = newstr;
		sb->capacity = newcapacity;
	}
	sb->str[sb->size] = c;
	sb->size++;
//	}
}
//char *string_buffer_to_string(string_buffer_t *sb) {//読み込んだ文字を文字列につなげる。
	
//}
void string_buffer_free(string_buffer_t *sb) {//文字列をフリーする。
	free ( sb->str );
	free ( sb );
}
int main(int argc, char** argv) {
	char *input = NULL;
	if ( argc == 2 ) {
		string_buffer_t *string = new_string_buffer ();
		int get_ch;
		char *fname = argv[1];
		FILE *fp = fopen ( fname, "r" );
		while( (get_ch = fgetc (fp) ) != EOF) {
//			if ( get_ch == '\n' ) {
//				printf("'Enter' has come\n");
//				input = string->str;
//				start (input);
//			} else {
			string_buffer_putc (string, get_ch);
//			}
		}
//		string_buffer_putc (string, '\0');
		input = string->str;
		printf("input = '%s'\n",input);
		start ( input );
		string_buffer_free ( string );
	} else if (argc == 1) {
		while((input = readline(">>> "))){
			start ( input );
		}
	}
//	fclose ( fp );
	return 0;
}
