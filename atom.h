#pragma once

/* Atom types */
#define ATOM_INT 0
#define ATOM_STRING 1
#define ATOM_LIST 2

typedef struct atom atom;
struct atom {
	int type;
	union {
		int i;
		char c;
		atom *next;
	};
};

atom *atom_create();
