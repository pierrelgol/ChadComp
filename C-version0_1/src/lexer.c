#include "../lib/lexer.h"

Lexer *lexer_init(char *stream, int stream_size)
{
	Lexer *self;

	if (!stream || stream_size == 0)
		return (NULL);

	self = (Lexer *) calloc(1, sizeof(Lexer));
	if (!self)
		return (NULL);
	self->size = stream_size;
	self->content = stream;
	return (self);
}

int lexer_consume(Lexer *self)
{
	if (!self || !self->content)
		return (false);
	if (self->cursor + 1 > self->size)
		return (EOF);
	if (self->content[self->cursor] == '\n')
	{
		self->line += 1;
		self->begline = self->cursor + 1;
	}
	self->cursor += 1;
	return (true);
}

int    lexer_trim(Lexer *self)
{
	int ch;

	if (!self || !self->content)
		return (false);
	ch = lexer_curr(self);
	while (ch != EOF && (ch == ' ' || ch == '\t'))
	{
		lexer_consume(self);
		ch = lexer_curr(self);
	}
	return (1);
}

int lexer_curr(Lexer *self)
{
	if (!self || !self->content)
		return (false);
	if (self->cursor >= self->size)
		return (EOF);
	return (self->content[self->cursor]);
}

int lexer_next(Lexer *self)
{
	if (!self || !self->content)
		return (0);
	if (self->cursor + 1 > self->size)
		return (EOF);
	return (self->content[self->cursor + 1]);
}

int lexer_prev(Lexer *self)
{
	if (!self || !self->content)
		return (0);
	if (self->cursor - 1 < 0)
		return (0);
	return (self->content[self->cursor - 1]);
}

int lexer_peek(Lexer *self, int index)
{
	if (!self || !self->content)
		return (0);
	if (self->cursor - index < 0 || self->cursor + index > self->size)
		return (0);
	return (self->content[self->cursor + index]);
}

Lexer *lexer_dispose(Lexer *self)
{
	if (self)
	{
		if (self->content)
			free(self->content);
		free(self);
	}
	return (NULL);
}
