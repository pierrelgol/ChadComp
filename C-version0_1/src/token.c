#include "../lib/token.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

Token *tokenizer_tokenize(Lexer *lex)
{
	Token *self;
	int    curr;
	int    next;

	if (!lex || !lex->content)
		return (NULL);
	self = (Token *) calloc(1, sizeof(Token));
	if (!self)
		return (NULL);
	self->line = lex->line;
	self->col = lex->cursor - lex->begline;
	self->ptr = &lex->content[lex->cursor];
	curr = lexer_curr(lex);
	next = lexer_next(lex);
	if (is_directive(curr))
	{
		self->kind = TOKEN_DIRECTIVE;
		self->ptr_len = lexer_consume(lex);
		return (self);
	}
	else if (curr == '"' && isascii(next))
	{
		self->kind = TOKEN_LITERAL;
		self->ptr_len = lexer_consume(lex);
		while ((curr = lexer_curr(lex)) != '"')
		{
			self->ptr_len += lexer_consume(lex);
		}
		self->ptr_len += lexer_consume(lex);
	}
	else if (curr == '/' && (next == '/' || next == '*'))
	{
		self->kind = TOKEN_COMMENT;
		self->ptr_len += lexer_consume(lex);
		self->ptr_len += lexer_consume(lex);
		while ((curr = lexer_curr(lex)) != '\n')
		{
			self->ptr_len += lexer_consume(lex);
		}
		lexer_consume(lex);
	}
	else if (is_operator(curr))
	{
		self->kind = TOKEN_OPERATOR;
		self->ptr_len = lexer_consume(lex);
		return (self);
	}
	else if (is_punctuator(curr))
	{
		self->kind = TOKEN_PUNCTUATOR;
		self->ptr_len = lexer_consume(lex);
		return (self);
	}
	else if (is_number(curr))
	{
		self->kind = TOKEN_NUMBER;
		self->ptr_len = lexer_consume(lex);
		return (self);
	}
	else if (is_identifier(curr))
	{
		self->kind = TOKEN_IDENTIFIER;
		while (is_identifier(curr = lexer_curr(lex)))
		{
			self->ptr_len += lexer_consume(lex);
		}
		if (is_keyword(self) == true)
		{
			self->kind = TOKEN_KEYWORD;
		}

		return (self);
	}
	else if (is_whitespace(curr))
	{
		while (is_whitespace(curr = lexer_curr(lex)))
		{
			self->ptr_len += lexer_consume(lex);
		}
		self->kind = TOKEN_IGNORE;
	}
	else
	{
		self->ptr_len += lexer_consume(lex);
		self->kind = TOKEN_IGNORE;
	}
	if (lex->cursor == lex->size)
	{
		self->kind = TOKEN_END;
	}

	return (self);
}

int is_keyword(Token *tok)
{
	if (strncmp("char", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("short", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("int", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("long", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("unisgned", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("signed", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("float", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("double", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("bool", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("_Bool", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("struct", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("enum", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("union", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("return", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("while", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("goto", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("if", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("do", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("for", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("typedef", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("sizeof", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("main", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("const", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("volatile", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("extern", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("static", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("auto", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("main", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("void", tok->ptr, tok->ptr_len) == 0)
		return (true);
	if (strncmp("else", tok->ptr, tok->ptr_len) == 0)
		return (true);
	return (false);
}

int is_whitespace(int n)
{
	return (isspace(n));
}

int is_directive(int n)
{
	return (n == '#');
}

int is_identifier(int n)
{
	return (n == '_' || isalnum(n));
}

int is_operator(int n)
{
	switch (n)
	{
	case '+':
		return (true);
		break;
	case '.':
		return (true);
		break;
	case '=':
		return (true);
		break;
	case '-':
		return (true);
		break;
	case '/':
		return (true);
		break;
	case '%':
		return (true);
		break;
	case '*':
		return (true);
		break;
	case '(':
		return (true);
		break;
	case ')':
		return (true);
		break;
	case '[':
		return (true);
		break;
	case ']':
		return (true);
		break;
	case '{':
		return (true);
		break;
	case '}':
		return (true);
		break;
	case '>':
		return (true);
		break;
	case '<':
		return (true);
		break;
	case '&':
		return (true);
		break;
	case '|':
		return (true);
		break;
	case '~':
		return (true);
		break;
	case '!':
		return (true);
		break;
	case '^':
		return (true);
		break;
	default:
		return (false);
	}
}

int is_comment(int n)
{
	return (n == '/');
}

int is_punctuator(int n)
{
	switch (n)
	{
	case ';':
		return (true);
		break;
	case ',':
		return (true);
		break;
	case '"':
		return (true);
		break;
	case '\'':
		return (true);
		break;
	case ':':
		return (true);
		break;
	default:
		return (false);
	}
}

int is_number(int n)
{
	return (isdigit(n));
}
