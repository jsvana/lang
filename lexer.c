#include "defines.h"
#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_NIL 0

token *token_create() {
	token *t = malloc(sizeof(token));
	if (!t) {
		return NIL;
	}

	t->type = TOKEN_NIL;
	t->next = NIL;

	return t;
}

static token *next_token(token *t) {
	if (!t) {
		return NIL;
	}

	token *tn = token_create();
	if (!tn) {
		return NIL;
	}

	tn->prev = t;
	t->next = tn;
	return tn;
}

int next_token_is(token *t, int type) {
	if (!t) {
		return FALSE;
	}

	return t->type == type;
}

void token_print(token *t) {
	if (!t) {
		return;
	}

	while (t) {
		if (t->type == TOKEN_INT) {
			printf(" %d", t->i);
		} else if (t->type == TOKEN_STRING) {
			printf(" %s", string_to_cstr(t->s));
		} else if (t->type == TOKEN_L_PAREN) {
			printf(" (");
		} else if (t->type == TOKEN_R_PAREN) {
			printf(" )");
		}

		t = t->next;
	}
	printf("\n");
}

int token_has_next(token *t) {
	if (!t) {
		return FALSE;
	}

	return t->next != NIL;
}

token *lex(char *input) {
	if (!input) {
		return NIL;
	}

	token *t = token_create();
	token *begin = t;
	char num[TOKEN_INT_MAX_LEN + 1];
	num[TOKEN_INT_MAX_LEN] = 0;
	int num_digits;

	// Begin taking in character by character
	while (*input) {
		char c = *input;
		if (t->type == TOKEN_INT) {
			if (isdigit(c) && num_digits < TOKEN_INT_MAX_LEN) {
				num[num_digits] = c;
				++num_digits;
				if (!*(input + 1) || num_digits == TOKEN_INT_MAX_LEN) {
					t->i = atoi(num);
					t = next_token(t);
				}
			} else {
				if (num_digits < TOKEN_INT_MAX_LEN) {
					num[num_digits] = 0;
				}
				t->i = atoi(num);

				t = next_token(t);
				input--;
			}
		} else if (t->type == TOKEN_STRING) {
			if (isspace(c) || c == '(' || c == ')') {
				t = next_token(t);
				input--;
			} else {
				string_add_char(t->s, c);
			}
		} else {
			if (c == '(') {
				t->type = TOKEN_L_PAREN;
				t = next_token(t);
			} else if (c == ')') {
				t->type = TOKEN_R_PAREN;
				t = next_token(t);
			} else if (isdigit(c)) {
				t->type = TOKEN_INT;
				num_digits = 0;
				input--;
			} else if (isspace(c)) {
				t = next_token(t);
			} else {//if (isalpha(c)) {
				t->type = TOKEN_STRING;
				t->s = string_create();
				if (!t->s) {
					// TODO: Do something with error
				}
				string_add_char(t->s, c);
			}
		}
		input++;
	}

	return begin;
}
