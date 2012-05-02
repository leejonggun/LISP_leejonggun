#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "lisp.h"

const char *type_name[] = {"PUSH","PUSH_V", "POP", "ADD", "SUB", "MUL", "DIV", "SML", "BIG", "EQL", "IF", "END", "CALL"};
void start (const char *input) {
	int tmp = 0;
	node_t *root_from_tokenize = NULL;
	int calc_result = 0;
	jindex = 0;
	Comp_flag = 0;
	func_call_flag = 0;
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
			opline_t *check_opline = codegen (root_from_tokenize);
			opline_t *opline = check_opline;
			while ( check_opline != NULL ) {
				if (check_opline->type == IF ) {
					printf("check_opline->op, check_opline->type = '%d, %s' has op_T'%d' and op_F'%d'.\n",check_opline->op, type_name[check_opline->type],check_opline->op_T->op,check_opline->op_F->op);
					check_opline = check_opline->op_T;//op_Tもop_Fもちゃんとある。op_Tの"END"にop_Fをつないだ。
				} else if (check_opline->type == CALL ) {
					printf("check_opline->op_T->op='%d'\n",check_opline->op_T->op);
					check_opline = check_opline->next;
				} else {
					printf("check_opline->op, check_opline->type = '%d, %s'\n",check_opline->op, type_name[check_opline->type]);
					check_opline = check_opline->next;
				}
			}
			if (opline == NULL)
				break;
			/*vm生成部分*/
			/*vm実行部分(oplineが命令列のトップ)*/

			printf("vm_run start\n");
			opline_t *run_opline = vm_run (opline, stack);
			if ( run_opline->type == SML || run_opline->type == BIG || run_opline->type == EQL ) {
				if ( run_opline->op == 1 )
					printf("the result after comparing is run_opline->op = 'T'\n");
				else if (run_opline->op == 0 )
				printf("the result after comparing is run_opline->op = 'Nil'\n");
			} else {
			printf("the result is run_opline->op ='%d'\n",run_opline->op);
			}
			/*vm実行部分*/
			break;
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
