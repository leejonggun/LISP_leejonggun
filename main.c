#include <stdio.h>
#include "lisp.h"
#include <readline/readline.h>


int main(int argc, char** argv) {
	char *input = NULL;
	while((input = readline(">>> "))){
		int tmp = 0;
		node_t *node_from_tokenize = NULL;
		root = NULL;
		prev_list = NULL;
		sym_data = NULL;
		operater_data = 0;
		num_data = 0;
		jindex = 0;
		while (tmp != (strlen(input))) {
			node_from_tokenize = tokenize(input, tmp);
			tmp = jindex;
		}
		print_node(node_from_tokenize);
		printf("Before free_node(root)\n");
		free_node(node_from_tokenize);
		printf("After free_node(root)\n");
	}
	return 0;
}


