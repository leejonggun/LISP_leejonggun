#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "lisp.h"

char *type_name[] = { "PUSH", "POP", "ADD" };
void start (const char *input) {
	int tmp = 0;
	node_t *root_from_tokenize = NULL;
	int calc_result = 0;
	jindex = 0;
	Comp_flag = 0;
	func_call_flag = 0;
	vm_top = NULL;//the root of cons tree.
	while (tmp != (strlen(input))) {
		root_from_tokenize = tokenize(input);
		tmp = jindex;
		printf("tokenize finished, tmp = '%d'\n", tmp);
		if ( root_from_tokenize == NULL ) {
			printf("ERROR in start\n");
			break;
		}
		if ( root_from_tokenize != NULL ) {

			/*vm生成部分*/
			opline_t *opline = make_vm (root_from_tokenize);
			while ( opline != NULL ) {
			printf("opline->op, opline->type = '%d, %s'\n",opline->op, type_name[opline->type]);
			opline = opline->next;
			}
			break;
			/*vm生成部分*/
			//print_node(root_from_tokenize);
			calc_result = eval (root_from_tokenize);
			if ( func_call_flag ) {
				if (Comp_flag) {
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
				} else {
				printf("After Function ='%d'\n",calc_result);
				}
			} else if ( Comp_flag ) {
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
			} else if ( calc_result == -1 ) {
				printf("Something wrong in eval.c\n");
			} else if ( quit_flag ) {
				break;
			} else {
				printf("After eval, calc_result = '%d'\n",calc_result);
			}	
			free_node(root_from_tokenize);
//			if ( func_call_flag == 1 ) //関数呼び出しの計算が終わったらフリーする。必要なかった。
//				free(defun_table->prev);
		} else {
			printf("The tokenizer doesn't work. Please debug tokenize.c\n");
		}
		while ( input[tmp] == ' ' || input[tmp] == '\n' ) {
			tmp++;
		}
	}
}
