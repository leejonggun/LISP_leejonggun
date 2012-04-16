#include <stdio.h>
#include "lisp.h"
#include <readline/readline.h>


int main(int argc, char** argv) {
	char *input = NULL;
	while((input = readline(">>> "))){
		int tmp = 0;
		node_t *root_from_tokenize = NULL;
		int calc_result = 0;
		jindex = 0;
		while (tmp != (strlen(input))) {
			root_from_tokenize = tokenize(input);
			tmp = jindex;
			printf("tokenize finished, tmp = '%d'\n", tmp);
			if ( root_from_tokenize == NULL ) {
				printf("ERROR in main\n");
				return 0;
			}
		}
		print_node(root_from_tokenize);
		printf("After print_node(root)\n");
		calc_result = eval (root_from_tokenize);
		printf("After eval, calc_result = '%d'\n",calc_result);
		free_node(root_from_tokenize);
		printf("After free_node(root)\n");
	}
	return 0;
}
