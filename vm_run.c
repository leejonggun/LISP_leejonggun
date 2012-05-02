#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lisp.h"

int* calculate (int *sp, opline_t *opline);
char* compare (int *sp, opline_t *opline);

int stack[STACK_SIZE];
//int *sp = &stack[0];

opline_t* vm_run ( opline_t *opline, int *stack) {
	char *cmp;
	int *sp = &stack[0];
	opline_t *result = (opline_t*) malloc (sizeof (opline_t) );//これがないとa label can only be part of a statement and a declaration is not a statementとエラー。
	while ( opline != NULL ) {
		switch (opline->type) {
			case PUSH: {
				*sp = opline->op;
				sp++;
				break;
			}
			case PUSH_V:
					//sp = stack[opline->op];
					*sp = stack[opline->op];
					sp++;
					break;
			case ADD:
			case SUB:
			case MUL:
			case DIV:
				sp = calculate (sp, opline);
				sp++;
				result->type = PUSH;
				//result->op = stack[0];
				//result->next = NULL;
				break;
			case SML:
			case BIG:
			case EQL:
				cmp = compare (sp, opline);
				result->type = opline->type;
				result->next = NULL;
				if ( strcmp (cmp, "T") == 0 )
					result->op = 1;
				else
					result->op = 0;
				break;
			case IF:
				if ( result->op == 1 )
					//result = vm_run ( opline->op_T, sp);
					opline = opline->op_T;
				else if ( result->op == 0 )
					//result = vm_run ( opline->op_F, sp);
					opline = opline->op_F;
				return result;
			case END: {
				result->type = PUSH;
				result->op = stack[0];
				result->next = NULL;
				return result;
			}
			case CALL:
				result = vm_run(opline->op_T, sp);
				sp -= opline->op_T->args_num;
				*sp = result;
				sp++;
				break;	

			default:
				break;
		}
		opline = opline->next;
	}
	return result;
}
int* calculate (int *sp, opline_t *opline) {
	sp--;
	int rhs = *sp;
	sp--;
	int lhs = *sp;
	switch (opline->type) {
		case ADD:
			*sp = lhs + rhs;
			return sp;
		case SUB:
			*sp = lhs - rhs;
			return sp;
		case MUL:
			*sp = lhs * rhs;
			return sp;
		case DIV:
			*sp = lhs / rhs;
			return sp;
		default:
			printf("The calculator doesn't work\n");
			return 0;
	}
}
char* compare (int *sp, opline_t *opline) {
	sp--;
	int rhs = *sp;
	sp--;
	int lhs = *sp;
	char *cmp_result = NULL;
	switch (opline->type) {
		case SML:
			if ( lhs < rhs )
				cmp_result = "T";
			else
				cmp_result = "Nil";
			return cmp_result;
		case BIG:
			if ( lhs > rhs )
				cmp_result = "T";
			else
				cmp_result = "Nil";
			return cmp_result;
		case EQL:
			if ( lhs == rhs )
				cmp_result = "T";
			else
				cmp_result = "Nil";
			return cmp_result;
		default:
			printf("The comparison doesn't work\n");
			return cmp_result;
	}
}
