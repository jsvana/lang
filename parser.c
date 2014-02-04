#include "parser.h"

#include "defines.h"

static token *parse_token(env *e, token *t, s_exp *s) {
	return t;
}

s_exp *parse(env *e, token *t) {
	if (!t) {
		return NIL;
	}

	parse_token(e, t, e->root);
	return e->root;
}
