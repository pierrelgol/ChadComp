#include "../lib/lx_token.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static char Keywords[32][10] = {
    {"auto"},     {"break"},   {"case"},   {"char"},     {"const"},
    {"continue"}, {"default"}, {"do"},     {"double"},   {"else"},
    {"enum"},     {"extern"},  {"float"},  {"for"},      {"goto"},
    {"if"},       {"int"},     {"long"},   {"register"}, {"return"},
    {"short"},    {"signed"},  {"sizeof"}, {"static"},   {"struct"},
    {"switch"},   {"typedef"}, {"union"},  {"unsigned"}, {"void"},
    {"volatile"}, {"while"}};

static TokenIdentity KeywordsKind[32] = {
    TOKEN_KWORD_AUTO,     TOKEN_KWORD_BREAK,    TOKEN_KWORD_CASE,
    TOKEN_KWORD_CHAR,     TOKEN_KWORD_CONST,    TOKEN_KWORD_CONTINUE,
    TOKEN_KWORD_DEFAULT,  TOKEN_KWORD_DO,       TOKEN_KWORD_DOUBLE,
    TOKEN_KWORD_ELSE,     TOKEN_KWORD_ENUM,     TOKEN_KWORD_EXTERN,
    TOKEN_KWORD_FLOAT,    TOKEN_KWORD_FOR,      TOKEN_KWORD_GOTO,
    TOKEN_KWORD_IF,       TOKEN_KWORD_INT,      TOKEN_KWORD_LONG,
    TOKEN_KWORD_REGISTER, TOKEN_KWORD_RETURN,   TOKEN_KWORD_SHORT,
    TOKEN_KWORD_SIGNED,   TOKEN_KWORD_SIZEOF,   TOKEN_KWORD_STATIC,
    TOKEN_KWORD_STRUCT,   TOKEN_KWORD_SWITCH,   TOKEN_KWORD_TYPEDEF,
    TOKEN_KWORD_UNION,    TOKEN_KWORD_UNSIGNED, TOKEN_KWORD_VOID,
    TOKEN_KWORD_VOLATILE, TOKEN_KWORD_WHILE,
};

static int KeywordsSize[32] = {
    0, 0, 0, 1, 0, 0, 0, 0, 8, 0, 2, 0, 4, 0, 0, 0,
    4, 8, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0,
};

static char Operator[40][8] = {
    [0] = "+",   [1] = "-",   [2] = "*",   [3] = "/",   [4] = "%",
    [5] = "!",   [6] = ".",   [7] = "=",   [8] = "&",   [9] = "~",
    [10] = "|",  [11] = "^",  [12] = "<",  [13] = ">",  [14] = "sizeof",
    [15] = "->", [16] = "++", [17] = "--", [18] = "+=", [19] = "-=",
    [20] = "*=", [21] = "/=", [22] = "%=", [23] = "!=", [24] = "==",
    [25] = "~=", [26] = "&=", [27] = "|=", [28] = "^=", [29] = "<=",
    [30] = ">=", [31] = "<<", [32] = ">>", [33] = "&&", [34] = "||",
    [35] = "(",  [36] = ")",  [37] = "[",  [38] = "]"};

static TokenIdentity OperatorKind[40] = {
    [0] = TOKEN_OPE_PLUS,
    [1] = TOKEN_OPE_MINUS,
    [2] = TOKEN_OPE_MULT,
    [3] = TOKEN_OPE_DIV,
    [4] = TOKEN_OPE_MOD,
    [5] = TOKEN_OPE_NOT,
    [6] = TOKEN_OPE_DOT,
    [7] = TOKEN_OPE_EQ,
    [8] = TOKEN_OPE_AND,
    [9] = TOKEN_OPE_BITWISE_TILDE,
    [10] = TOKEN_OPE_BITWISE_OR,
    [11] = TOKEN_OPE_BITWISE_XOR,
    [12] = TOKEN_OPE_LESS_THAN,
    [13] = TOKEN_OPE_MORE_THAN,
    [14] = TOKEN_OPE_SIZEOF,
    [15] = TOKEN_OPE_STRUCT_PTR,
    [16] = TOKEN_OPE_PLUS_PLUS,
    [17] = TOKEN_OPE_MINUS_MINUS,
    [18] = TOKEN_OPE_PLUS_EQ,
    [19] = TOKEN_OPE_MINUS_EQ,
    [20] = TOKEN_OPE_MULT_EQ,
    [21] = TOKEN_OPE_DIV_EQ,
    [22] = TOKEN_OPE_MOD_EQ,
    [23] = TOKEN_OPE_NOT_EQ,
    [24] = TOKEN_OPE_EQ_EQ,
    [25] = TOKEN_OPE_BITWISE_TILDE_EQ,
    [26] = TOKEN_OPE_BITWISE_AND_EQ,
    [27] = TOKEN_OPE_BITWISE_OR_EQ,
    [28] = TOKEN_OPE_BITWISE_XOR_EQ,
    [29] = TOKEN_OPE_LESS_THAN_EQ,
    [30] = TOKEN_OPE_MORE_THAN_EQ,
    [31] = TOKEN_OPE_BITWISE_LSHIFT,
    [32] = TOKEN_OPE_BITWISE_RSHIFT,
    [33] = TOKEN_OPE_LOGICAL_AND,
    [34] = TOKEN_OPE_LOGICAL_OR,
    [35] = TOKEN_OPE_OPEN_PARANTHESE,
    [36] = TOKEN_OPE_CLOSE_PARANTHESE,
    [37] = TOKEN_OPE_OPEN_SQUARE_BRACKETS,
    [38] = TOKEN_OPE_CLOSE_SQUARE_BRACKETS,
};

static int punctuator[128][1] = {
    [';'][0] = TOKEN_SEP_SEMI_COLON,
    [':'][0] = TOKEN_SEP_COLON,
    ['{'][0] = TOKEN_SEP_OPEN_CURLY_BRACKETS,
    ['}'][0] = TOKEN_SEP_CLOSE_CURLY_BRACKETS,
    [44][0] = TOKEN_SEP_SINGLE_QUOTE,
    ['"'][0] = TOKEN_SEP_DOUBLE_QUOTE,
};

Token *token_make(Scanner *scan, FnMem *mem)
{
	Token      *token;
	LexemeLooks looks;

	token = mem->alloc(1, sizeof(Token));
	if (!token)
		return (NULL);
	token->ptr = &scan->content[scan->cursor];
	token->row_start = scan->begline;
	token->col_start = scan->cursor - scan->begline;
	looks = lexeme_looks(token, scan);
	token->kind = token_identity(token, scan, looks);
	return (token);
}

int tokncmp(char *token1, char *token2, int n)
{
	while (*token1 && *token1 == *token2 && --n)
	{
		token1++;
		token2++;
	}
	return (*token1 - *token2);
}

int is_alpha(int ch)
{
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

int is_ascii(int ch)
{
	return (ch >= 0 && ch <= 127);
}

int is_digit(int ch)
{
	return (ch >= '0' && ch <= '9');
}

int is_alnum(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return (true);
	if (ch >= '0' && ch <= '9')
		return (true);
	return (false);
}

int is_space(int ch)
{
	if (ch == ' ' || ch == '\r' || ch == '\v')
		return (true);
	if (ch == '\f' || ch == '\t' || ch == '\n')
		return (true);
	return (false);
}

int is_newln(int ch)
{
	return (ch == '\n');
}

int is_identifier_start(int ch)
{
	return (is_alpha(ch) || ch == '_');
}

int is_identifier_inside(int ch)
{
	return (is_alnum(ch) || ch == '_');
}

int is_operator(Token *token)
{
	int i;

	i = 39;
	while (i--)
	{
		if (tokncmp(Operator[i], token->ptr, token->ptr_len) == 0)
		{
			token->kind = OperatorKind[i];
			return (true);
		}
	}
	return (false);
}

int is_keyword(Token *token)
{
	int i;

	i = 32;
	while (i--)
	{
		if (tokncmp(token->ptr, Keywords[i], token->ptr_len) == 0)
		{
			token->kind = KeywordsKind[i];
			token->kind_size = KeywordsSize[i];
			return (true);
		}
	}
	return (false);
}

int is_comment_start(int ch1, int ch2)
{
	if (ch1 == '/')
	{
		if (ch2 == '/')
			return (true);
		if (ch2 == '*')
			return (true);
	}
	return (false);
}

int is_single_comment(int ch1, int ch2)
{
	return (ch1 == '/' && ch2 == '/');
}

int is_multi_comment(int ch1, int ch2)
{
	return (ch1 == '/' && ch2 == '*');
}

int lexeme_looks_directive(int ch1)
{
	return (ch1 == '#');
}

int lexeme_looks_comment(int ch1, int ch2)
{
	if (ch1 == '/')
	{
		if (ch2 == '/')
			return (true);
		if (ch2 == '*')
			return (true);
	}
	return (false);
}

int lexeme_looks_number_literal(int ch1)
{
	return (ch1 >= '0' && ch1 <= '9');
}

int lexeme_looks_string_literal(int ch1)
{
	return (ch1 == '"');
}

int lexeme_looks_ignoreable(int ch)
{
	return (is_space(ch));
}

int lexeme_looks_operator(int ch1)
{
	switch (ch1)
	{
	case '!':
		return (true);
	case '&':
		return (true);
	case '%':
		return (true);
	case '+':
		return (true);
	case '-':
		return (true);
	case '*':
		return (true);
	case '/':
		return (true);
	case '=':
		return (true);
	case '~':
		return (true);
	case '^':
		return (true);
	case '|':
		return (true);
	case '<':
		return (true);
	case '>':
		return (true);
	case '.':
		return (true);
	case '(':
		return (true);
	case ')':
		return (true);
	case '[':
		return (true);
	case ']':
		return (true);
	case '?':
		return (true);
	case ':':
		return (true);
	default:
		return (false);
	}
}

int lexeme_looks_punctuator(int ch1)
{
	switch (ch1)
	{
	case '"':
		return (true);
	case 39:
		return (true);
	case 44:
		return (true);
	case ';':
		return (true);
	case ':':
		return (true);
	case '{':
		return (true);
	case '}':
		return (true);
	default:
		return (false);
	}
}

int lexeme_looks_identifier(int ch1)
{
	return (is_alpha(ch1) || ch1 == '_');
}

LexemeLooks lexeme_looks(Token *token, Scanner *scan)
{
	int ch1;
	int ch2;

	(void) token;
	ch1 = scanner_curr(scan);
	ch2 = scanner_next(scan);

	if (lexeme_looks_directive(ch1))
		return (LEXEME_LOOKS_DIRECTIVE);
	else if (lexeme_looks_number_literal(ch1))
		return (LEXEME_LOOKS_NUMBER_LITERAL);
	else if (lexeme_looks_string_literal(ch1))
		return (LEXEME_LOOKS_STRING_LITERAL);
	else if (lexeme_looks_identifier(ch1) || ch1 == '_')
		return (LEXEME_LOOKS_IDENTIFIER);
	else if (lexeme_looks_comment(ch1, ch2))
		return (LEXEME_LOOKS_COMMENT);
	else if (lexeme_looks_operator(ch1))
		return (LEXEME_LOOKS_OPERATOR);
	else if (lexeme_looks_punctuator(ch1))
		return (LEXEME_LOOKS_PUNCTUATOR);
	else if (lexeme_looks_ignoreable(ch1))
		return (LEXEME_LOOKS_IGNOREABLE);
	else
		return (LEXEME_LOOKS_UNRESOLVED);
}

TokenIdentity token_identity(Token *token, Scanner *scan, LexemeLooks looks)
{
	int ch1;
	int ch2;

	ch1 = scanner_curr(scan);
	ch2 = scanner_next(scan);
	if (looks == LEXEME_LOOKS_DIRECTIVE)
	{
		token->kind = TOKEN_DIRECTIVE;
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_consume(scan);
		while ((ch1 = scanner_curr(scan)) != EOF)
		{
			token->ptr_len += 1;
			token->col_end += 1;
			scanner_consume(scan);
			if (is_newln(ch1))
				break;
		}
		return (TOKEN_DIRECTIVE);
	}

	if (looks == LEXEME_LOOKS_COMMENT)
	{
		token->ptr_len += 2;
		token->col_end += 2;
		scanner_consume(scan);
		if (is_single_comment(ch1, ch2))
		{
			token->kind = TOKEN_SINGLE_LINE_COMMENT;
			while ((ch1 = scanner_curr(scan)) != EOF)
			{
				token->ptr_len += 1;
				token->col_end += 1;
				if (is_newln(ch1))
					break;
				scanner_consume(scan);
			}
			scanner_consume(scan);
			return (TOKEN_SINGLE_LINE_COMMENT);
		}
		else if (is_multi_comment(ch1, ch2))
		{
			token->kind = TOKEN_MULTILINE_COMMENT;
			while ((ch1 = scanner_curr(scan)) != EOF)
			{
				ch2 = scanner_next(scan);
				token->ptr_len += 1;
				token->col_end += 1;
				if (ch1 == '*' && ch2 == '/')
					break;
				scanner_consume(scan);
			}
			token->row_end = scan->line;
			scanner_consume(scan);
			scanner_consume(scan);
			return (TOKEN_MULTILINE_COMMENT);
		}
		return (TOKEN_IGNORE);
	}

	if (looks == LEXEME_LOOKS_OPERATOR)
	{
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_consume(scan);
		while ((ch1 = scanner_curr(scan)) != EOF)
		{
			if (!lexeme_looks_operator(ch1))
				break;
			token->ptr_len += 1;
			token->col_end += 1;
			scanner_consume(scan);
		}
		is_operator(token);
		return (token->kind);
	}

	if (looks == LEXEME_LOOKS_STRING_LITERAL)
	{
		token->kind = TOKEN_STRING_LITERAL;
		token->ptr_len += 2;
		token->col_end += 2;
		scanner_consume(scan);
		while ((ch1 = scanner_curr(scan)) != EOF)
		{
			token->ptr_len += 1;
			token->col_end += 1;
			scanner_consume(scan);
			if (ch1 == '"')
				break;
		}
		scanner_consume(scan);
		return (TOKEN_STRING_LITERAL);
	}

	if (looks == LEXEME_LOOKS_PUNCTUATOR)
	{
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_consume(scan);
		return (token->kind);
	}

	if (looks == LEXEME_LOOKS_NUMBER_LITERAL)
	{
		token->kind = TOKEN_NUMBER_LITERAL;
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_consume(scan);

		while ((ch1 = scanner_curr(scan)) != EOF)
		{
			if (!is_digit(ch1) && ch1 != '.')
				break;
			token->ptr_len += 1;
			token->col_end += 1;
			scanner_consume(scan);
		}
		return (TOKEN_NUMBER_LITERAL);
	}

	if (looks == LEXEME_LOOKS_IDENTIFIER)
	{
		token->kind = TOKEN_IDENTIFIER;
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_consume(scan);
		while ((ch1 = scanner_curr(scan)) != EOF)
		{
			if (!is_identifier_inside(ch1))
				break;
			token->ptr_len += 1;
			token->col_end += 1;
			scanner_consume(scan);
		}
		if (is_keyword(token))
			return (token->kind);

		return (TOKEN_IDENTIFIER);
	}

	if (looks == LEXEME_LOOKS_IGNOREABLE)
	{
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_consume(scan);
		return (TOKEN_IGNORE);
	}

	if (looks == LEXEME_LOOKS_UNRESOLVED)
	{
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_consume(scan);
		return (TOKEN_ERROR);
	}

	token->ptr_len += 1;
	token->col_end += 1;
	scanner_consume(scan);
	return (TOKEN_EOF);
}
