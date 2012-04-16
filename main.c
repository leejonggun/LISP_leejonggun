#include <stdio.h>
#include "lisp.h"
#include <readline/readline.h>


int main(int argc, char** argv) {
	char *input = NULL;
	while((input = readline(">>> "))){
		int tmp = 0;
		node_t *root_from_tokenize = NULL;
//		node_t *calc_result = NULL;
		jindex = 0;
		while (tmp != (strlen(input))) {
			root_from_tokenize = tokenize(input, tmp);
			printf("tokenize finished, tmp = '%d'\n", tmp);
			tmp = jindex;
//			if ( root_from_tokenize == NULL ) {
//				printf("ERROR in main\n");
//				return 0;
//			}
		}
		print_node(root_from_tokenize);
		printf("After print_node(root)\n");
//		calc_result = eval ( root_from_tokenize );
//		printf("After eval\n");
		free_node(root_from_tokenize);
		printf("After free_node(root)\n");
	}
	return 0;
}
