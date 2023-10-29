#include "../lib/lx_memory.h"

FnMem *mem_init(void)
{
	FnMem *self;

	self = (FnMem *) malloc(sizeof(FnMem));
	if (!self)
		return (NULL);
	self->alloc = calloc;
	self->dealloc = free;
	self->borrow = mem_borrow;
	return (self);
}

void *mem_borrow(void **from)
{
	void *unique_ptr;

	unique_ptr = (*from);
	(*from) = NULL;

	return (unique_ptr);
}

void mem_dispose(FnMem *self)
{
	if (self)
		free(self);
}
