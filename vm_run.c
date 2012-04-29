#include <stdio.h>
#include <stdlib.h>
#include "lisp.h"

opline_t *run_add (opline_t *top_process, opline_t *opline );

opline_t* vm_run ( opline_t *opline ) {
	opline_t *top_process = opline;
	opline_t *add_result = NULL;//これがないとa label can only be part of a statement and a declaration is not a statementとエラー。
	while ( opline != NULL ) {
		switch (opline->type) {
			case ADD:
				add_result = run_add (top_process, opline);
				top_process = add_result;
				break;
			/*case SUB:
				break;
			case MUL:
				break;
			case DIV:
				break;
			case SML:
				break;
			case BIG:
				break;
			case EQL:
				break;
			case IF:
				break;
			case END:
				break;
			 */
			default:
				break;
		}
		opline = opline->next;
	}
	return top_process;
}
opline_t *run_add (opline_t *top_process, opline_t *opline) {
	opline_t *first_arg = top_process;
	opline_t *second_arg = top_process->next;
	first_arg->op += second_arg->op;
	//first_arg->next = opline->next;
	opline->type = PUSH;
	opline->op = first_arg->op;
	//free (first_arg);
	//free (second_arg);
	return opline;
}
