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

	t->next = tn;
	return tn;
}

int next_token_is(token *t, int type) {
	if (!t) {
		return FALSE;
	}

	return t->type == type;
}

char *token_get_str(token *t) {
	if (!t) {
		return NIL;
	}

	int capacity = 8, i = 0;
	token *iter = t;

	char *s = malloc(sizeof(char) * capacity);
	if (!s) {
		return NIL;
	}

	while (iter && iter->type == TOKEN_CHAR && iter->c != CHAR_NIL) {
		if (i == capacity) {
			capacity <<= 1;
			s = reallocf(s, sizeof(char) * capacity);
			if (!s) {
				return NIL;
			}
		}

		s[i] = iter->c;
		++i;
		iter = iter->next;
	}

	return s;
}

int token_streq(token *t, const char *str) {
	if (!t || !str) {
		return FALSE;
	}

	int i = 0, len = strlen(str);
	token *iter = t;

	while (iter && iter->type == TOKEN_CHAR && i < len) {
		if (str[i] != iter->c) {
			return FALSE;
		}
		++i;
		iter = iter->next;
	}

	return i == len;
}

void token_print(token *t) {
	if (!t) {
		return;
	}

	while (t) {
		if (t->type == TOKEN_INT) {
			printf(" %d", t->i);
		} else if (t->type == TOKEN_CHAR) {
			if (t->c == CHAR_NIL) {
				printf(" \\0");
			} else {
				printf(" %c", t->c);
			}
		} else if (t->type == TOKEN_L_PAREN) {
			printf(" (");
		} else if (t->type == TOKEN_R_PAREN) {
			printf(" )");
		} else if (t->type == TOKEN_OP_PLUS) {
			printf(" +");
		} else if (t->type == TOKEN_OP_MINUS) {
			printf(" -");
		} else if (t->type == TOKEN_OP_SPLAT) {
			printf(" *");
		} else if (t->type == TOKEN_OP_SLASH) {
			printf(" /");
		} else if (t->type == TOKEN_OP_TICK) {
			printf(" '");
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
			} else if (isalpha(c)) {
				t->type = TOKEN_CHAR;
				t->c = c;
				t = next_token(t);
				if ((*(input + 1) && !isalpha(*(input + 1))) || !*(input + 1)) {
					t->type = TOKEN_CHAR;
					t->c = CHAR_NIL;
					t = next_token(t);
				}
			} else if (c == '+') {
				t->type = TOKEN_OP_PLUS;
				t = next_token(t);
			} else if (c == '-') {
				t->type = TOKEN_OP_MINUS;
				t = next_token(t);
			} else if (c == '*') {
				t->type = TOKEN_OP_SPLAT;
				t = next_token(t);
			} else if (c == '/') {
				t->type = TOKEN_OP_SLASH;
				t = next_token(t);
			} else if (c == '\'') {
				t->type = TOKEN_OP_TICK;
				t = next_token(t);
			} else {
				/* Skip spaces */
			}
		}
		input++;
	}

	return begin;
}
