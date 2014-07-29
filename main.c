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

func_ret *cadd(env *e, s_arg *arg) {
	func_ret *ret = func_ret_create();
	if (!ret) {
		return NIL;
	}

	if (!arg) {
		return ret;
	}

	ret->type = FUNC_RET_INT;
	ret->i = 0;

	while (arg) {
		if (arg->type == S_ARG_ATOM) {
			if (arg->a->type == ATOM_INT) {
				ret->i += arg->a->i;
			} else {
				char *type = atom_type_string(arg->a);
				fprintf(stderr, "Incorrect type, needed int but received %s\n", type);
				free(type);
				return NIL;
			}
		} else if (arg->type == S_ARG_VAR) {
			// Look up symbol in table and evaluate
			ret->i += 0;
		} else if (arg->type == S_ARG_S_EXP) {
			// Eval s_exp
			ret->i += 0;
		} else {
			fprintf(stderr, "Bad argument");
			return NIL;
		}

		arg = arg->next;
	}

	return ret;
}

func_ret *csub(env *e, s_arg *arg) {
	return NIL;
}

func_ret *cmul(env *e, s_arg *arg) {
	return NIL;
}

func_ret *cdiv(env *e, s_arg *arg) {
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
		}
	}

	return 0;
}
