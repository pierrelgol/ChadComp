#ifndef LX_TOKEN_H
#define LX_TOKEN_H

#include "lx_memory.h"
#include "lx_scanner.h"

typedef enum
{
	TOKEN_END,
	TOKEN_ERROR,
	TOKEN_DIRECTIVE,
	TOKEN_IDENTIFIER,
	TOKEN_COMMENT,
	TOKEN_LITERAL,
	TOKEN_NUMBER,
	TOKEN_PUNCTUATOR,
	TOKEN_OPERATOR,
	TOKEN_KEYWORD,

} TokenKind;

typedef struct
{
	TokenKind kind;
	int       row_start;
	int       row_end;
	int       col_start;
	int       col_end;
	int       size_of_kind;
	int       ptr_len;
	char     *ptr;

} Token;

Token *token_make(Scanner *scan, FnMem *mem);
int    tokncmp(char *token1, char *token2, int n);
int    is_ascii(int ch);
int    is_alpha(int ch);
int    is_digit(int ch);
int    is_alnum(int ch);
int    is_space(int ch);
int    is_newln(int ch);
int    is_directive(int ch);
int    is_identifier_start(int ch);
int    is_identifier_inside(int ch);
int    is_punctuator(int ch);
int    is_operator(int ch);
int    is_keyword(char *str, int len);
int    is_comment_start(int ch1, int ch2);
int    is_single_comment(int ch1, int ch2);
int    is_multi_comment(int ch1, int ch2);

int       is_error(int ch);
TokenKind token_kind(Token *token, Scanner *scan);

#endif
