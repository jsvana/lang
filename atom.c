#include "atom.h"

#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string *string_create() {
	string *s = malloc(sizeof(string));
	if (!s) {
		return NIL;
	}

	s->len = 0;
	s->cap = 8;

	s->str = malloc(sizeof(char) * s->cap);
	if (!s->str) {
		free(s);
		return NIL;
	}

	memset(s->str, 0, s->cap);

	return s;
}

string *string_add_char(string *s, char c) {
	if (!s) {
		return NIL;
	}

	if (s->len == s->cap) {
		s->cap *= 2;
		s->str = reallocf(s->str, sizeof(char) * s->cap);
		if (s->str) {
			free(s);
			return NIL;
		}
	}

	s->str[s->len] = c;
	++s->len;

	return s;
}

char *string_to_cstr(string *s) {
	if (!s) {
		return NIL;
	}

	return s->str;
}

char *atom_type_string(atom *a) {
	char *ret;

	if (!a) {
		return NIL;
	}

	if (a->type == ATOM_INT) {
		asprintf(&ret, "int");
	} else if (a->type == ATOM_STRING) {
		asprintf(&ret, "string");
	} else {
		asprintf(&ret, "unknown");
	}

	return ret;
}

atom *atom_create() {
	atom *a = malloc(sizeof(atom));
	return a;
}
