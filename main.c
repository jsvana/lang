#include "atom.h"
#include "defines.h"
#include "func.h"
#include "lexer.h"
#include "s_exp.h"
#include "parser.h"

#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

#define PROMPT "> "

/* Parser states */
#define STATE_NIL 0
#define STATE_HAS_OPEN_PAREN 1
#define STATE_HAS_OP 2
#define STATE_HAS_ARG1 3
#define STATE_HAS_ARG2 4
#define STATE_HAS_CLOSE_PAREN 5

/*
char *parse_input(char *input, s_exp *s) {
	int parse_state = STATE_NIL;

	while (*input) {
		char c = *input;
		switch (c) {
			case '(':
				if (parse_state == STATE_HAS_OP) {
					s->arg1.type = S_ARG_S_EXP;
					s->arg1.s = s_exp_create();
					if (!s->arg1.s) {
						fprintf(stderr, "Error creating s_exp (char: %c)\n", c);
						return NIL;
					}
					input = parse_input(input, s->arg1.s);
					input--;
					parse_state = STATE_HAS_ARG1;
				} else if (parse_state == STATE_HAS_ARG1) {
					s->arg2.type = S_ARG_S_EXP;
					s->arg2.s = s_exp_create();
					if (!s->arg2.s) {
						fprintf(stderr, "Error creating s_exp (char: %c)\n", c);
						return NIL;
					}
					input = parse_input(input, s->arg2.s);
					input--;
					parse_state = STATE_HAS_ARG2;
				} else if (parse_state == STATE_NIL) {
					parse_state = STATE_HAS_OPEN_PAREN;
				} else if (parse_state == STATE_HAS_OPEN_PAREN) {
					fprintf(stderr, "Bad state, need op first (char: %c)\n", c);
					return NIL;
				}
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				if (parse_state != STATE_HAS_OPEN_PAREN) {
					fprintf(stderr, "Bad state, need paren first (char: %c)\n", c);
					return NIL;
				}
				parse_state = STATE_HAS_OP;
				//s->opcode = //get_op(c);
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				// I'll think of a better way eventually
				if (parse_state == STATE_HAS_OP) {
					parse_state = STATE_HAS_ARG1;
					s->arg1.type = S_ARG_ATOM;
					s->arg1.a = atom_create();
					s->arg1.a->type = ATOM_INT;
					s->arg1.a->i = c - '0';
				} else if (parse_state == STATE_HAS_ARG1) {
					parse_state = STATE_HAS_ARG2;
					s->arg2.type = S_ARG_ATOM;
					s->arg2.a = atom_create();
					s->arg2.a->type = ATOM_INT;
					s->arg2.a->i = c - '0';
				} else {
					fprintf(stderr, "Bad state, need op first (char: %c)\n", c);
					return NIL;
				}
				break;
			case ')':
				if (parse_state != STATE_HAS_ARG2) {
					fprintf(stderr, "Bad state, need two args first (char: %c)\n", c);
					return NIL;
				}
				parse_state = STATE_HAS_CLOSE_PAREN;
				input++;
				return input;
				break;
			default: // Skip space
				break;
		}
		input++;
	}

	if (parse_state != STATE_HAS_CLOSE_PAREN) {
		fprintf(stderr, "Bad state, need closing paren, (char: %c)\n", *input);
		return NIL;
	}

	return input;
}

s_exp_res *eval_s_exp(s_exp *s) {
	if (!s) {
		return NIL;
	}

	int a, b;
	if (s->arg1.type == S_ARG_ATOM) {
		a = s->arg1.a->i;
	} else {
		s_exp_res *res = eval_s_exp(s->arg1.s);
		a = res->i;
	}

	if (s->arg2.type == S_ARG_ATOM) {
		b = s->arg2.a->i;
	} else {
		s_exp_res *res = eval_s_exp(s->arg2.s);
		b = res->i;
	}

	s_exp_res *ret = s_exp_res_create();
	if (!ret) {
		return NIL;
	}

	ret->type = S_EXP_RES_INT;

	switch (s->opcode) {
		case OP_ADD:
			ret->i = a + b;
			break;
		case OP_SUB:
			ret->i = a - b;
			break;
		case OP_MUL:
			ret->i = a * b;
			break;
		case OP_DIV:
			ret->i = a / b;
			break;
	}

	return ret;
}
*/

func_ret *cadd(void *args) {
	func_ret *ret = func_ret_create();
	if (!ret) {
		return NIL;
	}

	if (!args) {
		return ret;
	}

	int *iargs = args;
	ret->type = FUNC_RET_INT;
	ret->i = 0;
	for (int i = 1; i < iargs[0] + 1; i++) {
		ret->i += iargs[i];
	}

	return ret;
}

func_ret *csub(void *args) {
	return NIL;
}

func_ret *cmul(void *args) {
	return NIL;
}

func_ret *cdiv(void *args) {
	return NIL;
}

void env_bootstrap(env *e) {
	if (!e) {
		return;
	}

	env_add_cfunc(e, "+", &cadd);
	env_add_cfunc(e, "-", &csub);
	env_add_cfunc(e, "*", &cmul);
	env_add_cfunc(e, "/", &cdiv);
}

int main(int argc, char **argv) {
	int running = TRUE;

	using_history();

	env *e = env_create();

	env_bootstrap(e);

	int args[] = { 5, 1, 2, 3, 4, 5 };
	func_ret *r = ccall(e, "+", args);
	if (r && r->type == FUNC_RET_INT) {
		printf("res: %d\n", r->i);
	} else {
		printf("oh noes\n");
	}

	while (running) {
		char *input = readline(PROMPT);
		if (!input) {
			fprintf(stdout, "\n");
			running = FALSE;
			break;
		}
		add_history(input);
		token *t = lex(input);
		if (t) {
			token_print(t);
			char *s = token_get_str(t);
			if (s) {
				printf("string: %s\n", s);
			}
			if (token_streq(t, "def")) {
				printf("define function\n");
			}
		}
		/*
		s_exp *s = s_exp_create();
		if (!s) {
			fprintf(stderr, "Error creating s-exp\n");
			exit(1);
		}
		if (parse_input(input, s)) {
			//print_s_exp(s);
			s_exp_res *r = eval_s_exp(s);
			if (!r) {
				fprintf(stderr, "Error evaluating s_expression\n");
				exit(1);
			}
			printf("%d\n", r->i);
		} else {
			// Shit's broke, yo
		}
		*/
		free(input);
	}

	return 0;
}
