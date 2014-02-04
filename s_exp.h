#pragma once

#include "atom.h"
#include "defines.h"
#include "func.h"

/* s_arg types */
#define S_ARG_ATOM 0
#define S_ARG_S_EXP 1

/* s_exp_res types */
#define S_EXP_RES_INT 0

/* Opcodes */
#define OP_NIL 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4

#define FUNC_TABLE_SIZE 256

typedef struct s_arg s_arg;
struct s_arg {
	int type;
	union {
		atom *a;
		s_exp *s;
	};
};

struct s_exp {
	int opcode;
	s_arg *args;
	int arg_count;
	s_exp *next;
};

typedef struct env env;
struct env {
	s_exp *root;
	func funcs[FUNC_TABLE_SIZE];
	int func_count;
};

typedef struct s_exp_res s_exp_res;
struct s_exp_res {
	int type;
	union {
		int i;
	};
	s_exp_res *next;
};

s_exp *s_exp_create();
s_arg *s_arg_create();
s_exp_res *s_exp_res_create();

env *env_create();

func_ret *ccall(env *e, const char *func, void *args);

void env_add_cfunc(env *e, const char *name, cfunc *f);
void env_add_lfunc(env *e, const char *name, s_exp *l);

void print_s_exp(s_exp *s);

s_exp_res *eval_s_exp(s_exp *s);
