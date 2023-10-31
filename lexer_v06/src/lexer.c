#include "lib/lexer.h"

static Token	cbuffer[CBUFFER_SIZE];

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
lexer_dispose(Lexer *self);
