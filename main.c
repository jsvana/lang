#include "atom.h"
#include "defines.h"
#include "lexer.h"

#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

#define PROMPT "> "

/* Parser states */
#define STATE_NIL 0
#define STATE_HAS_OPEN_PAREN 1
#define STATE_HAS_OP 2
#define STATE_HAS_ARG1 3
#define STATE_HAS_ARG2 4
#define STATE_HAS_CLOSE_PAREN 5

/* s_arg types */
#define S_ARG_ATOM 0
#define S_ARG_S_EXP 1

/* Opcodes */
#define OP_NIL 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4

/* s_exp_res types */
#define S_EXP_RES_INT 0

typedef struct s_exp s_exp;

typedef struct s_arg s_arg;
struct s_arg {
	int type;
	union {
		atom *a;
		s_exp *s;
	};
};

struct s_exp {
	int opcode;
	s_arg arg1;
	s_arg arg2;
	s_exp *next;
};

typedef struct s_exp_res s_exp_res;
struct s_exp_res {
	int type;
	union {
		int i;
	};
};

s_exp *s_exp_create() {
	s_exp *s = malloc(sizeof(s_exp));
	if (!s) {
		return NIL;
	}

	s->opcode = OP_NIL;
	s->next = NIL;

	return s;
}

s_exp_res *s_exp_res_create() {
	s_exp_res *sr = malloc(sizeof(s_exp_res));
	return sr;
}

void compress_whitespace(char *input) {
	if (!input) {
		return;
	}

	// TODO: Eliminate newlines and tabs, compress spaces into one space
}

int get_op(char c) {
	switch (c) {
		case '+':
			return OP_ADD;
		case '-':
			return OP_SUB;
		case '*':
			return OP_MUL;
		case '/':
			return OP_DIV;
		default:
			return OP_NIL;
	}
}

char opcode_to_char(int opcode) {
	switch (opcode) {
		case OP_ADD:
			return '+';
		case OP_SUB:
			return '-';
		case OP_MUL:
			return '*';
		case OP_DIV:
			return '/';
		default:
			return '?';
	}
}

void print_s_exp(s_exp *s) {
	if (!s) {
		return;
	}

	printf("(%c", opcode_to_char(s->opcode));
	fflush(stdout);

	printf(" [%d]", s->arg1.type);
	if (s->arg1.type == S_ARG_ATOM) {
		printf(" %d", s->arg1.a->i);
	} else {
		printf(" ");
		print_s_exp(s->arg1.s);
	}

	printf(" [%d]", s->arg2.type);
	if (s->arg2.type == S_ARG_ATOM) {
		printf(" %d", s->arg2.a->i);
	} else {
		printf(" ");
		print_s_exp(s->arg2.s);
	}

	printf(")");
}

char *parse_input(char *input, s_exp *s) {
	int parse_state = STATE_NIL;

	while (*input) {
		char c = *input;
		switch (c) {
			case '(':
				if (parse_state == STATE_HAS_OP) {
					s->arg1.type = S_ARG_S_EXP;
					s->arg1.s = s_exp_create();
					if (!s->arg1.s) {
						fprintf(stderr, "Error creating s_exp (char: %c)\n", c);
						return NIL;
					}
					input = parse_input(input, s->arg1.s);
					input--;
					parse_state = STATE_HAS_ARG1;
				} else if (parse_state == STATE_HAS_ARG1) {
					s->arg2.type = S_ARG_S_EXP;
					s->arg2.s = s_exp_create();
					if (!s->arg2.s) {
						fprintf(stderr, "Error creating s_exp (char: %c)\n", c);
						return NIL;
					}
					input = parse_input(input, s->arg2.s);
					input--;
					parse_state = STATE_HAS_ARG2;
				} else if (parse_state == STATE_NIL) {
					parse_state = STATE_HAS_OPEN_PAREN;
				} else if (parse_state == STATE_HAS_OPEN_PAREN) {
					fprintf(stderr, "Bad state, need op first (char: %c)\n", c);
					return NIL;
				}
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				if (parse_state != STATE_HAS_OPEN_PAREN) {
					fprintf(stderr, "Bad state, need paren first (char: %c)\n", c);
					return NIL;
				}
				parse_state = STATE_HAS_OP;
				s->opcode = get_op(c);
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				// I'll think of a better way eventually
				if (parse_state == STATE_HAS_OP) {
					parse_state = STATE_HAS_ARG1;
					s->arg1.type = S_ARG_ATOM;
					s->arg1.a = atom_create();
					s->arg1.a->type = ATOM_INT;
					s->arg1.a->i = c - '0';
				} else if (parse_state == STATE_HAS_ARG1) {
					parse_state = STATE_HAS_ARG2;
					s->arg2.type = S_ARG_ATOM;
					s->arg2.a = atom_create();
					s->arg2.a->type = ATOM_INT;
					s->arg2.a->i = c - '0';
				} else {
					fprintf(stderr, "Bad state, need op first (char: %c)\n", c);
					return NIL;
				}
				break;
			case ')':
				if (parse_state != STATE_HAS_ARG2) {
					fprintf(stderr, "Bad state, need two args first (char: %c)\n", c);
					return NIL;
				}
				parse_state = STATE_HAS_CLOSE_PAREN;
				input++;
				return input;
				break;
			default: // Skip space
				break;
		}
		input++;
	}

	if (parse_state != STATE_HAS_CLOSE_PAREN) {
		fprintf(stderr, "Bad state, need closing paren, (char: %c)\n", *input);
		return NIL;
	}

	return input;
}

s_exp_res *eval_s_exp(s_exp *s) {
	if (!s) {
		return NIL;
	}

	int a, b;
	if (s->arg1.type == S_ARG_ATOM) {
		a = s->arg1.a->i;
	} else {
		s_exp_res *res = eval_s_exp(s->arg1.s);
		a = res->i;
	}

	if (s->arg2.type == S_ARG_ATOM) {
		b = s->arg2.a->i;
	} else {
		s_exp_res *res = eval_s_exp(s->arg2.s);
		b = res->i;
	}

	s_exp_res *ret = s_exp_res_create();
	if (!ret) {
		return NIL;
	}

	ret->type = S_EXP_RES_INT;

	switch (s->opcode) {
		case OP_ADD:
			ret->i = a + b;
			break;
		case OP_SUB:
			ret->i = a - b;
			break;
		case OP_MUL:
			ret->i = a * b;
			break;
		case OP_DIV:
			ret->i = a / b;
			break;
	}

	return ret;
}

int main(int argc, char **argv) {
	int running = TRUE;

	using_history();

	while (running) {
		char *input = readline(PROMPT);
		if (!input) {
			fprintf(stdout, "\n");
			running = FALSE;
			break;
		}
		add_history(input);
		token *t = lex(input);
		if (t) {
			token_print(t);
			char *s = token_get_str(t);
			if (s) {
				printf("string: %s\n", s);
			}
			if (token_streq(t, "def")) {
				printf("define function\n");
			}
		}
		/*
		s_exp *s = s_exp_create();
		if (!s) {
			fprintf(stderr, "Error creating s-exp\n");
			exit(1);
		}
		if (parse_input(input, s)) {
			//print_s_exp(s);
			s_exp_res *r = eval_s_exp(s);
			if (!r) {
				fprintf(stderr, "Error evaluating s_expression\n");
				exit(1);
			}
			printf("%d\n", r->i);
		} else {
			// Shit's broke, yo
		}
		*/
		free(input);
	}

	return 0;
}
