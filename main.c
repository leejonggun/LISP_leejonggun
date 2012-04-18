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
				break;
			}
		}
		if ( root_from_tokenize != NULL ) {
		print_node(root_from_tokenize);
		printf("After print_node(root)\n");
		calc_result = eval (root_from_tokenize);
		if ( calc_result == 0 )	//to do Truely, the result of comparison is not '0' and '1' but 't' and 'Nil'.
			printf("calc_result ='%d', comp_result =False!\n",calc_result);
		else if ( calc_result == 1 )
			printf("calc_result ='%d', comp_result = True!\n",calc_result);
		else if ( calc_result == -1 )
			printf("calc_result ='%d'. This is ERROR Message, means Something wrong!\n",calc_result);
		else
			printf("After eval, calc_result = '%d'\n",calc_result);

		free_node(root_from_tokenize);
		printf("After free_node(root)\n");
		} else {
			printf("The tokenizer doesn't work. Please debug tokenize.c\n");
//			return 0;
		}
	}
	return 0;
}
