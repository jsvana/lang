#pragma once

#include "lexer.h"
#include "s_exp.h"

s_exp *parse(env *e, token *t);
