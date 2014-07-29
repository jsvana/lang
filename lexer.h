#pragma once

#include "atom.h"

/* Token types */
#define TOKEN_NIL 0
#define TOKEN_INT 1
#define TOKEN_STRING 2
#define TOKEN_R_PAREN 3
#define TOKEN_L_PAREN 4

/*
 * Redesigned lexer:
 * Parse for parens (l and r)
 * If not a paren, parse for numbers and begin gathering the number
 * If not a number, parse for whitespace and ignore
 * If not whitespace, assume string and parse until whitespace
 */

#define TOKEN_INT_MAX_LEN 10

typedef struct token token;
struct token {
	int type;
	union {
		int i;
		string *s;
	};
	token *prev;
	token *next;
};

token *token_create();
void token_print(token *t);
int token_has_next(token *t);

token *lex(char *input);
