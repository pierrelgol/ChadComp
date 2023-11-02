#include "lib/lexer.h"
#include <stdio.h>

static Token cbuffer[CBUFFER_SIZE];

Lexer
*lexer_init(Scanner *scanner, MemRes *memory_manager, const char *path)
{
	Lexer	*self;

	if (!scanner || !memory_manager | !path)
		return (NULL);

	self                 = memory_manager->alloc(1, sizeof(Lexer));
	self->path           = path;
	self->scan           = scanner;
	self->memory_manager = memory_manager;
	self->buffer_size    = CBUFFER_SIZE;
	self->token_buffer   = cbuffer;
	self->token_count    = 0;

	return (self);
}

Token
*lexer_rewind_token(Lexer *self)
{
	self->token_count -= 1;
	return (&self->token_buffer[self->token_count % self->buffer_size]);
}

Token
*lexer_eat_token(Lexer *self)
{
	self->token_count += 1;
	return (&self->token_buffer[self->token_count % self->buffer_size]);
}

Token
*lexer_peek_curr_token(Lexer *self)
{
	return (&self->token_buffer[self->token_count % self->buffer_size]);
}

Token
*lexer_peek_prev_token(Lexer *self)
{
	return (&self->token_buffer[(self->token_count - 1) % self->buffer_size]);
}

Token
*lexer_peek_next_token(Lexer *self)
{
	return (&self->token_buffer[(self->token_count + 1) % self->buffer_size]);
}
	
void
lexer_dispose(Lexer *self)
{
	if (self)
		self->memory_manager->dealloc(self);
}


Token
*lexer_produce_token(Lexer *self)
{
	Token 	*token;
	MemRes	*mem;
	Scanner *scan;
	int	ch1;
	int	ch2;
	int	ch3;

	mem = self->memory_manager;
	scan = self->scan;
	token = (Token *) mem->alloc(1, sizeof(Token));
	scanner_skip(scan, is_whitespace);
	if (!token)
	{
		fprintf(stderr, "Error : Lexer failed to produce new token!\n");
		return (NULL);
	}
	token->col_start = scan->current_cursor_position - scan->current_line_begin;
	token->line_start = scan->current_line;
	token->ptr = &scan->scanner_content[scan->current_cursor_position];

	ch1 = scanner_peek_curr_character(scan);
	ch2 = scanner_peek_next_character(scan);
	scanner_eat_character(scan);
	ch3 = scanner_peek_next_character(scan);
	scanner_rewind_character(scan);

	if (is_directive(ch1))
	{
		token->kind = TOK_DIRECTIVE;
		token->ptr_len += 1;
		token->col_end += 1;
		while ((ch1 = scanner_peek_curr_character(scan)) != EOF)
		{
			if (is_newline(ch1))
				break;
			scanner_eat_character(scan);
			token->ptr_len += 1;
			token->col_end += 1;
		}
		scanner_eat_character(scan);
		return (token);
	}

	if (is_comment_start(ch1, ch2))
	{
		if (ch1 == '/' && ch2 == '/')
		{
			token->ptr_len += 2;
			token->col_end += 2;
			scanner_eat_character(scan);
			scanner_eat_character(scan);
			while ((ch1 = scanner_peek_curr_character(scan)) != EOF)
			{
				if (is_single_line_commment_end(ch1))
					break;
				scanner_eat_character(scan);
				token->ptr_len += 1;
				token->col_end += 1;
			}
			token->kind = TOK_SINGLL_COMMENT;
			scanner_eat_character(scan);
			return (token);
		}
		else
		{
			token->ptr_len += 2;
			token->col_end += 2;
			scanner_eat_character(scan);
			scanner_eat_character(scan);
			while ((ch1 = scanner_peek_curr_character(scan)) != EOF)
			{
				if (is_multi_line_commment_end(ch1, ch2))
					break;
				scanner_eat_character(scan);
				ch2 = scanner_peek_next_character(scan);
				token->ptr_len += 1;
				token->col_end += 1;
			}
			token->kind = TOK_MULTIL_COMMENT;
			scanner_eat_character(scan);
			scanner_eat_character(scan);
			return (token);
		}
	}

	if (is_identifier_start(ch1))
	{
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_eat_character(scan);
		while ((ch1 = scanner_peek_curr_character(scan)) != EOF)
		{
			if (!is_identifier_inside(ch1))
				break;
			scanner_eat_character(scan);
			token->ptr_len += 1;
			token->col_end += 1;
		}
		if (is_keyword(token))
		{
			scanner_eat_character(scan);
			return (token);
		}
		scanner_eat_character(scan);
		return (token);
	}

	if (is_digit(ch1))
	{
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_eat_character(scan);
		while ((ch1 = scanner_peek_curr_character(scan)) != EOF)
		{
			if (!is_number_literal_inside(ch1))
				break;
			scanner_eat_character(scan);
			token->ptr_len += 1;
			token->col_end += 1;
		}
		scanner_eat_character(scan);
		token->kind = TOK_NUM_LITERAL;
		return (token);
	}

	if (is_string_literal_start(ch1))
	{	
		token->ptr_len += 1;
		token->col_end += 1;
		scanner_eat_character(scan);
		while ((ch1 = scanner_peek_curr_character(scan)) != EOF)
		{
			if (is_string_literal_end(ch1))
				break;
			scanner_eat_character(scan);
			token->ptr_len += 1;
			token->col_end += 1;
		}
		scanner_eat_character(scan);
		token->kind = TOK_STR_LITERAL;
		return (token);
	}
	return (token);

	if (scanner_peek_curr_character(scan) == EOF)
	{
		token->kind = TOK_END;
		token->ptr_len += 1;
		token->col_end += 1;
		return (token);
	}
	else
	{
		scanner_eat_character(scan);
		token->kind = TOK_ERROR;
		token->ptr_len += 1;
		token->col_end += 1;
		return (token);
	}
	__builtin_unreachable();
}

int 	
is_newline(int n)
{
	return (n == '\n');
}

int 	
is_whitespace(int n)
{
	switch (n) {
		case 9         : return (true);
		case 11 ... 13 : return (true);
        	case ' '       : return (true);
		default	       : return (false);
        }
}

int 	
is_alpha(int n)
{
	switch (n) {
		case 'a'...'z' : return (true);
		case 'A'...'Z' : return (true);
		default        : return (false);
        }
}

int 	
is_alnum(int n)
{
	switch (n) {
		case '0'...'9' : return (true);
		case 'a'...'z' : return (true);
		case 'A'...'Z' : return (true);
		default        : return (false);
        }
}

int 	
is_digit(int n)
{	
	switch (n) {
		case '0'...'9' : return (true);
		default        : return (false);
        }
}

TokenKind 	
is_operator(int n1, int n2, int n3)
{
	switch (n1) {
		case '+' : {
			switch(n2) {
				case '+' : return (TOK_OP_PLUS_PLUS);
				case '=' : return (TOK_OP_PLUS_EQ);
			}
		}
		case '-' : switch(n2){
			case '=' : return (TOK_OP_MINUS_EQ);
		}
		case '*' : switch(n2){
			case '=' : return (TOK_OP_MULT_EQ);
			default  : return (TOK_OP_MULT);
		}
		case '/' : switch(n2){
			case '=' : return (TOK_OP_DIV_EQ);
			default  : return (TOK_OP_DIV);
		}
		case '%' : switch(n2){
			case '=' : return (TOK_OP_MOD_EQ);
			default  : return (TOK_OP_MOD);
		}
		case '=' : switch(n2){
			case '=' : return (TOK_OP_EQ_EQ);
			default  : return (TOK_OP_EQ);
		}
		case '&' : switch(n2){
			case '=' : return (TOK_OP_AND_EQ);
			default  : return (TOK_OP_AMPERSAND);
		}
		case '|' : switch(n2){
			case '=' : return (TOK_OP_OR_EQ);
			default  : return (TOK_OP_OR);
		}
		case '^' : switch(n2){
			case '=' : return (TOK_OP_XOR_EQ);
			default  : return (TOK_OP_XOR);
		}
		case '!' : switch(n2){
			case '=' : return (TOK_OP_NOT_EQ);
			default  : return (TOK_OP_NOT);
		}
		case '(' : return (TOK_OP_L_PAR);
		case ')' : return (TOK_OP_R_PAR);
		case '.' : return (TOK_OP_DOT);
		case '[' : return (TOK_OP_L_BRACK);
		case ']' : return (TOK_OP_R_BRACK);
		case '~' : switch(n2){
			case '=' : return (TOK_OP_TILDE_EQ);
			default  : return (TOK_OP_TILDE);
		}
		case '<' : switch(n2){
			case '=' : return (TOK_OP_LCHEV_EQ);
			case '<' : switch (n3) {
                        	case '=' : return (TOK_OP_2LCHEV_EQ);
				default  : return (TOK_OP_2LCHEV);
                        }
			default  : return (TOK_OP_L_CHEV);
		}
		case '>' : switch(n2){
			case '=' : return (TOK_OP_RCHEV_EQ);
			case '>' : switch (n3) {
                        	case '=' : return (TOK_OP_2RCHEV_EQ);
				default  : return (TOK_OP_2RCHEV);
                        }
			default  : return (TOK_OP_R_CHEV);
		}
		case ':' : return (TOK_OP_COLON);
		case '?' : return (TOK_OP_QMARK);        
		default  : return (false);
        }
}

int 	
is_punctuator(int n)
{
	switch (n) {
		case '{' : return (true);
		case '}' : return (true);
		case ';' : return (true);
		case ',' : return (true);
		default  : return (false);
        }
}

int 	
is_identifier_start(int n)
{
	switch (n) {
		case '_'       : return (true);
		case 'A'...'Z' : return (true);
		case 'a'...'z' : return (true);
		default	       : return (false);
        }
}

int 	
is_identifier_inside(int n)
{
	switch (n) {
		case '_'       : return (true);
		case 'A'...'Z' : return (true);
		case 'a'...'z' : return (true);
		case '0'...'9' : return (true);
		default	       : return (false);
        }
}

int 	
is_comment_start(int n1, int n2)
{
	switch (n1) {
        	case '/' : {
			switch (n2) {
                        	case '/' : return (true);
                        	case '*' : return (true);
				default  : return (false); 
                        }
		}
		default : return (false);
        }
}

int 	
is_single_line_commment_end(int n)
{
	return (n == '\n');
}

int 	
is_multi_line_commment_end(int n1, int n2)
{
	if (n1 == '*' && n2 == '/')
		return (true);
	return (false);
}

int 	
is_directive(int n)
{
	if (n == '#')
		return (true);
	return (false);
}

int
is_string_literal_start(int n)
{
	return (n == '"');
}

int
is_string_literal_end(int n)
{
	return (n == '"' || n == '\\');
}

int
is_number_literal_inside(int n)
{
	switch (n) {
        	case '.'       : return (true);
        	case '0'...'9' : return (true);
		default	       : return (false);
        }
}

int	
tokncmp(char *s1, char *s2, int n)
{
	while (*s1 && *s1 == *s2 && --n)
	{
		++s1;
		++s2;
	}
	return (*s1 - *s2);
}

TokenKind 
is_keyword(Token *token)
{
	switch (token->ptr_len) {
		case 2 : {
			if (tokncmp(token->ptr, "if", token->ptr_len) == 0)
				return (TOK_KW_IF);
			if (tokncmp(token->ptr, "do", token->ptr_len) == 0)
				return (TOK_KW_DO);
		}
		case 3 : {
			if (tokncmp(token->ptr, "int", token->ptr_len) == 0)
				return (TOK_KW_INT);
			if (tokncmp(token->ptr, "for", token->ptr_len) == 0)
				return (TOK_KW_FOR);
		}
		case 4 : {
			if (tokncmp(token->ptr, "auto", token->ptr_len) == 0)
				return (TOK_KW_AUTO);
			if (tokncmp(token->ptr, "case", token->ptr_len) == 0)
				return (TOK_KW_CASE);
			if (tokncmp(token->ptr, "char", token->ptr_len) == 0)
				return (TOK_KW_CHAR);
			if (tokncmp(token->ptr, "else", token->ptr_len) == 0)
				return (TOK_KW_ELSE);
			if (tokncmp(token->ptr, "enum", token->ptr_len) == 0)
				return (TOK_KW_ENUM);
			if (tokncmp(token->ptr, "goto", token->ptr_len) == 0)
				return (TOK_KW_GOTO);
			if (tokncmp(token->ptr, "long", token->ptr_len) == 0)
				return (TOK_KW_LONG);
			if (tokncmp(token->ptr, "void", token->ptr_len) == 0)
				return (TOK_KW_VOID);
		}
		case 5 : {
			if (tokncmp(token->ptr, "break", token->ptr_len) == 0)
				return (TOK_KW_BREAK);
			if (tokncmp(token->ptr, "const", token->ptr_len) == 0)
				return (TOK_KW_CONST);
			if (tokncmp(token->ptr, "float", token->ptr_len) == 0)
				return (TOK_KW_FLOAT);
			if (tokncmp(token->ptr, "short", token->ptr_len) == 0)
				return (TOK_KW_SHORT);
			if (tokncmp(token->ptr, "union", token->ptr_len) == 0)
				return (TOK_KW_UNION);
			if (tokncmp(token->ptr, "while", token->ptr_len) == 0)
				return (TOK_KW_WHILE);
		}
		case 6 : {
			if (tokncmp(token->ptr, "double", token->ptr_len) == 0)
				return (TOK_KW_DOUBLE);
			if (tokncmp(token->ptr, "extern", token->ptr_len) == 0)
				return (TOK_KW_EXTERN);
			if (tokncmp(token->ptr, "return", token->ptr_len) == 0)
				return (TOK_KW_RETURN);
			if (tokncmp(token->ptr, "signed", token->ptr_len) == 0)
				return (TOK_KW_SIGNED);
			if (tokncmp(token->ptr, "sizeof", token->ptr_len) == 0)
				return (TOK_KW_SIZEOF);
			if (tokncmp(token->ptr, "static", token->ptr_len) == 0)
				return (TOK_KW_STATIC);
			if (tokncmp(token->ptr, "struct", token->ptr_len) == 0)
				return (TOK_KW_STRUCT);
			if (tokncmp(token->ptr, "switch", token->ptr_len) == 0)
				return (TOK_KW_SWITCH);			
		}
		case 7 : {
			if (tokncmp(token->ptr, "default", token->ptr_len) == 0)
				return (TOK_KW_DEFAULT);
			if (tokncmp(token->ptr, "typedef", token->ptr_len) == 0)
				return (TOK_KW_TYPEDEF);			
		}
		case 8 : {
			if (tokncmp(token->ptr, "continue", token->ptr_len) == 0)
				return (TOK_KW_CONTINUE);			
			if (tokncmp(token->ptr, "register", token->ptr_len) == 0)
				return (TOK_KW_REGISTER);			
			if (tokncmp(token->ptr, "restrict", token->ptr_len) == 0)
				return (TOK_KW_RESTRICT);			
			if (tokncmp(token->ptr, "unsigned", token->ptr_len) == 0)
				return (TOK_KW_UNSIGNED);			
			if (tokncmp(token->ptr, "volatile", token->ptr_len) == 0)
				return (TOK_KW_VOLATILE);			
			
		}
		default : return (token->kind);
        }
}
