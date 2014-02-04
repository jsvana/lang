#pragma once

#include "atom.h"

/* Token types */
#define TOKEN_NIL 0
#define TOKEN_INT 1
#define TOKEN_CHAR 2
#define TOKEN_LIST 3
#define TOKEN_FUNC 4
#define TOKEN_R_PAREN 5
#define TOKEN_L_PAREN 6
#define TOKEN_OP_PLUS 7
#define TOKEN_OP_MINUS 8
#define TOKEN_OP_SPLAT 9
#define TOKEN_OP_SLASH 10
#define TOKEN_OP_TICK 11

#define TOKEN_INT_MAX_LEN 10

typedef struct token token;
struct token {
	int type;
	union {
		int i;
		char c;
	};
	token *prev;
	token *next;
};

token *token_create();
void token_print(token *t);
int token_streq(token *t, const char *str);
char *token_get_str(token *t);
int token_has_next(token *t);

token *lex(char *input);
