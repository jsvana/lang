#pragma once

/* Function types */
#define FUNC_NIL 0
#define FUNC_C 1
#define FUNC_L 2

/* Function result types */
#define FUNC_RET_NIL 0
#define FUNC_RET_INT 1

typedef struct func_ret func_ret;
struct func_ret {
	int type;
	union {
		int i;
	};
};

typedef struct env env;
typedef struct s_arg s_arg;
typedef struct s_exp s_exp;

typedef func_ret *(cfunc)(env *e, s_arg *);

typedef struct func func;
struct func {
	char *name;
	int type;
	union {
		s_exp *s;
		cfunc *c;
	};
};

func *func_create();
void func_set_name(func *f, const char *name);
func_ret *func_ret_create();
