#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
	TOKEN_END = 0,
	TOKEN_UNKNOWN,
	TOKEN_IGNORE,
	TOKEN_IDENTIFIER,
	TOKEN_DIRECTIVE,
	TOKEN_OPERATOR,
	TOKEN_NUMBER,
	TOKEN_LITERAL,
	TOKEN_PUNCTUATOR,
	TOKEN_COMMENT,
	TOKEN_KEYWORD,

} TokenKind;

typedef struct
{
	TokenKind   kind;
	int         line;
	int         col;
	int         len;
	int         size;
	int         ptr_len;
	char       *ptr;
	const char *path;
} Token;

typedef struct
{
	int   line;
	int   size;
	int   cursor;
	int   begline;
	char *content;

} Lexer;

Lexer *lexer_init(char *stream, int stream_size);
int    lexer_consume(Lexer *self);
int    lexer_trim(Lexer *self);
int    lexer_next(Lexer *self);
int    lexer_curr(Lexer *self);
int    lexer_prev(Lexer *self);
int    lexer_peek(Lexer *self, int index);
Lexer *lexer_dispose(Lexer *self);

#endif