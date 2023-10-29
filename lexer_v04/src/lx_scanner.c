#include "../lib/lx_scanner.h"

Scanner *scanner_init(char *stream, int stream_size, FnMem *mem)
{
	Scanner *self;

	self = (Scanner *) mem->alloc(1, sizeof(Scanner));
	if (!self)
		return (NULL);
	self->mem = mem;
	self->size = stream_size;
	self->content = stream;
	return (self);
}

int scanner_consume(Scanner *self)
{
	if (!self)
		return (0);

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

int scanner_trim(Scanner *self)
{
	int ch = 0;

	if (!self)
		return (0);

	ch = scanner_curr(self);
	while (ch != EOF && (ch == ' ' || ch == '\t'))
	{
		scanner_consume(self);
		ch = scanner_curr(self);
	}
	return (1);
}

int scanner_curr(Scanner *self)
{
	if (!self)
		return (0);
	if (self->cursor >= self->size)
		return (EOF);
	return (self->content[self->cursor]);
}

int scanner_next(Scanner *self)
{
	if (!self)
		return (0);
	if (self->cursor + 1 > self->size)
		return (EOF);
	return (self->content[self->cursor + 1]);
}

int scanner_prev(Scanner *self)
{
	if (!self)
		return (0);
	if (self->cursor - 1 < 0)
		return (0);
	return (self->content[self->cursor - 1]);
}

int scanner_peek(Scanner *self, int index)
{
	if (!self)
		return (0);
	if (self->cursor - index < 0 || self->cursor + index > self->size)
		return (0);
	return (self->content[self->cursor + index]);
}

void scanner_dispose(Scanner *self)
{
	if (self)
	{
		if (self->content)
			self->mem->dealloc(self->content);
		self->mem->dealloc(self);
	}
}
