#include "s_exp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

s_exp *s_exp_create() {
	s_exp *s = malloc(sizeof(s_exp));
	if (!s) {
		return NIL;
	}

	s->opcode = OP_NIL;
	s->arg_count = 0;
	s->next = NIL;

	return s;
}

s_exp_res *s_exp_res_create() {
	s_exp_res *sr = malloc(sizeof(s_exp_res));
	if (!sr) {
		return NIL;
	}

	sr->next = NIL;

	return sr;
}

s_arg *s_arg_create() {
	s_arg *sa = malloc(sizeof(s_arg));
	if (!sa) {
		return NIL;
	}

	sa->type = S_ARG_NIL;

	return sa;
}

s_arg *s_arg_set_var(s_arg *sa, const char *name) {
	if (!sa || !name) {
		return NIL;
	}

	if (sa->type != S_ARG_NIL) {
		if (sa->type == S_ARG_ATOM) {
			free(sa->a);
		} else if (sa->type == S_ARG_S_EXP) {
			// TODO: Free s_exp
		} else if (sa->type == S_ARG_VAR) {
			free(sa->v);
		}
	}

	sa->type = S_ARG_VAR;
	sa->v = malloc(sizeof(char) * (strlen(name) + 1));
	if (!sa->v) {
		free(sa);
		return NIL;
	}

	strcpy(sa->v, name);

	return sa;
}

symbol *symbol_create() {
	symbol *s = malloc(sizeof(symbol));
	if (!s) {
		return NIL;
	}

	s->type = SYMBOL_NIL;

	return s;
}

env *env_create() {
	env *e = malloc(sizeof(env));
	if (!e) {
		return NIL;
	}

	e->root = s_exp_create();
	if (!e->root) {
		free(e);
		return NIL;
	}

	e->size = 0;
	e->cap = 8;
	e->symbols = malloc(sizeof(symbol) * e->cap);
	if (!e->symbols) {
		free(e->root);
		free(e);
		return NIL;
	}

	return e;
}

void env_add_cfunc(env *e, const char *name, cfunc *f) {
	if (!e || !f) {
		return;
	}

	if (e->size == e->cap) {
		// Resize
		e->cap *= 2;
		e->symbols = reallocf(e->symbols, sizeof(symbol) * e->cap);
		if (!e->symbols) {
			free(e->root);
			free(e);
			return;
		}
	}

	e->symbols[e->size].type = SYMBOL_FUNC;
	e->symbols[e->size].f = func_create();
	if (!e->symbols[e->size].f) {
		return;
	}
	e->symbols[e->size].f->type = FUNC_C;
	e->symbols[e->size].f->c = f;
	func_set_name(e->symbols[e->size].f, name);

	++e->size;
}

void env_add_lfunc(env *e, const char *name, s_exp *l) {
	if (!e || !l) {
		return;
	}

	if (e->size == e->cap) {
		// Resize
		e->cap *= 2;
		e->symbols = reallocf(e->symbols, sizeof(symbol) * e->cap);
		if (!e->symbols) {
			free(e->root);
			free(e);
			return;
		}
	}

	e->symbols[e->size].type = SYMBOL_FUNC;
	e->symbols[e->size].f = func_create();
	if (!e->symbols[e->size].f) {
		return;
	}
	e->symbols[e->size].f->type = FUNC_L;
	e->symbols[e->size].f->s = l;
	func_set_name(e->symbols[e->size].f, name);

	++e->size;
}

func_ret *ccall(env *e, const char *func, void *args) {
	if (!e || !func) {
		return NIL;
	}

	int i;
	for (i = 0; i < e->size; i++) {
		// TODO: Switch to symbols
		if (e->symbols[i].type == FUNC_L && strcmp(e->funcs[i].name, func) == 0) {
			// TODO: Evaluate s_exp with given arguments
			printf("Calling s_exp\n");
			return NIL;
		}
	}
	for (i = 0; i < e->func_count; i++) {
		if (e->funcs[i].type == FUNC_C && strcmp(e->funcs[i].name, func) == 0) {
			return (e->funcs[i].c)(args);
		}
	}
}

void print_s_exp(s_exp *s) {
	if (!s) {
		return;
	}

	//printf("(%c", opcode_to_char(s->opcode));
	fflush(stdout);

	s_arg *arg = s->args;
	while (arg) {
		printf(" [%d]", arg->type);
		if (arg->type == S_ARG_ATOM) {
			printf(" %d", arg->a->i);
		} else {
			printf(" ");
			print_s_exp(arg->s);
		}

		arg++;
	}

	printf(")");
}

s_exp_res *eval_s_exp(s_exp *s) {
	if (!s) {
		return NIL;
	}

	s_exp_res *ret = s_exp_res_create();
	if (!ret) {
		return NIL;
	}

	return ret;
}
