#pragma once

#include "atom.h"
#include "defines.h"
#include "func.h"

/* s_arg types */
#define S_ARG_NIL 0
#define S_ARG_ATOM 1
#define S_ARG_S_EXP 2
#define S_ARG_VAR 3

/* s_exp_res types */
#define S_EXP_RES_INT 0

/* Symbol types */
#define SYMBOL_NIL 0
#define SYMBOL_FUNC 1
#define SYMBOL_VAR 2

#define FUNC_TABLE_SIZE 256

typedef struct s_arg s_arg;
struct s_arg {
	int type;
	union {
		atom *a;
		s_exp *s;
		char *v;
	};
	s_arg *next;
};

struct s_exp {
	int opcode;
	s_arg *args;
	int arg_count;
	s_exp *next;
};

typedef struct var var;
struct var {
	char *name;
	atom *a;
};

typedef struct symbol symbol;
struct symbol {
	int type;
	union {
		atom *a;
		func *f;
	};
};

typedef struct env env;
struct env {
	s_exp *root;
	//func funcs[FUNC_TABLE_SIZE];
	symbol *symbols;
	int size;
	int cap;
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

s_arg *s_arg_set_var(s_arg *sa, const char *name);

symbol *symbol_create();

env *env_create();

func_ret *ccall(env *e, const char *func, void *args);

void env_add_cfunc(env *e, const char *name, cfunc *f);
void env_add_lfunc(env *e, const char *name, s_exp *l);

void print_s_exp(s_exp *s);

s_exp_res *eval_s_exp(s_exp *s);
