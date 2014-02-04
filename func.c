#include "func.h"

#include "defines.h"

#include <stdlib.h>
#include <string.h>

func *func_create() {
	func *f = malloc(sizeof(func));
	if (!f) {
		return NIL;
	}

	f->type = FUNC_NIL;

	return f;
}

void func_set_name(func *f, const char *name) {
	if (!f || !name) {
		return;
	}

	f->name = malloc(sizeof(char) * (strlen(name) + 1));
	if(!f->name) {
		free(f);
		return;
	}

	strcpy(f->name, name);
}

func_ret *func_ret_create() {
	func_ret *fr = malloc(sizeof(func_ret));
	if (!fr) {
		return NIL;
	}

	fr->type = FUNC_RET_NIL;

	return fr;
}
