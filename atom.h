#pragma once

/* Atom types */
#define ATOM_INT 0
#define ATOM_STRING 1
#define ATOM_LIST 2

typedef struct string string;
struct string {
	int len;
	int cap;
	char *str;
};

typedef struct atom atom;
struct atom {
	int type;
	union {
		int i;
		char c;
		atom *next;
	};
};

string *string_create();
string *string_add_char(string *s, char c);
char *string_to_cstr(string *s);

char *atom_type_string(atom *a);

atom *atom_create();
