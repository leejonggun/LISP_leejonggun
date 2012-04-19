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
		Comp_flag = 0;
		while (tmp != (strlen(input))) {
			root_from_tokenize = tokenize(input);
			tmp = jindex;
			printf("tokenize finished, tmp = '%d'\n", tmp);
			if ( root_from_tokenize == NULL ) {
				printf("ERROR in main\n");
				break;
			}
		}
	if ( root_from_tokenize != NULL ) {
		print_node(root_from_tokenize);
		printf("After print_node(root)\n");
		calc_result = eval (root_from_tokenize);
		if ( Comp_flag == 1 ) {
			switch (calc_result) {
				case 0:
					printf("After eval, Comp_flag is occered. comp_result = Nil.\n");
					break;
				case 1:
					printf("After eval, Comp_flag is occered. comp_result = True.\n");
					break;
				case -1:
					printf("After eval, Comp_flag is occered. But, Something wrong\n");
					break;
			}
		} else if ( Comp_flag == 0 ) {
			printf("After eval, calc_result = '%d'\n",calc_result);
		}
	free_node(root_from_tokenize);
	printf("After free_node(root)\n");
	} else {
			printf("The tokenizer doesn't work. Please debug tokenize.c\n");
//			return 0;
		}
	}
	return 0;
}
