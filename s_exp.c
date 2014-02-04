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

	e->func_count = 0;

	return e;
}

void env_add_cfunc(env *e, const char *name, cfunc *f) {
	if (!e || !f) {
		return;
	}

	if (e->func_count == FUNC_TABLE_SIZE) {
		return;
	}

	e->funcs[e->func_count].type = FUNC_C;
	e->funcs[e->func_count].c = f;
	func_set_name(&(e->funcs[e->func_count]), name);

	++e->func_count;
}

void env_add_lfunc(env *e, const char *name, s_exp *l) {
	if (!e || !l) {
		return;
	}

	if (e->func_count == FUNC_TABLE_SIZE) {
		return;
	}

	e->funcs[e->func_count].type = FUNC_L;
	e->funcs[e->func_count].s = l;
	func_set_name(&(e->funcs[e->func_count]), name);

	++e->func_count;
}

func_ret *ccall(env *e, const char *func, void *args) {
	if (!e || !func) {
		return NIL;
	}

	int i;
	for (i = 0; i < e->func_count; i++) {
		if (e->funcs[i].type == FUNC_L && strcmp(e->funcs[i].name, func) == 0) {
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

static int get_op(char c) {
	switch (c) {
		case '+':
			return OP_ADD;
		case '-':
			return OP_SUB;
		case '*':
			return OP_MUL;
		case '/':
			return OP_DIV;
		default:
			return OP_NIL;
	}
}

static char opcode_to_char(int opcode) {
	switch (opcode) {
		case OP_ADD:
			return '+';
		case OP_SUB:
			return '-';
		case OP_MUL:
			return '*';
		case OP_DIV:
			return '/';
		default:
			return '?';
	}
}

void print_s_exp(s_exp *s) {
	if (!s) {
		return;
	}

	printf("(%c", opcode_to_char(s->opcode));
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
