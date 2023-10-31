#include "lib/lexer.h"

MemRes 
*mem_init(void)
{
	MemRes *self;

	self = (MemRes*) malloc(sizeof(MemRes));
	if (!self)
		return (NULL);
	self->alloc = calloc;
	self->dealloc = free;
	self->borrow = mem_borrow;

	return (self);
}

unique_ptr 
mem_borrow(void **from)
{
	if (!from || !(*from))
		return (NULL);
	unique_ptr ownership;

	ownership = (*from);
	(*from) = NULL;
	return (ownership);
}

void
mem_dispose(MemRes *self)
{
	if (self)
		free (self);
}
