#ifndef TOKEN_H
#define TOKEN_H

#include "lexer.h"
#include <stdlib.h>

Token *tokenizer_tokenize(Lexer *lex);
int is_keyword(Token *tok);
int    is_directive(int n);
int    is_identifier(int n);
int    is_operator(int n);
int    is_punctuator(int n);
int    is_number(int n);
int    is_literal(int n);
int    is_whitespace(int n);
int    is_comment(int n);

#endif
