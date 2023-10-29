#ifndef LX_MEMORY_H
#define LX_MEMORY_H

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

typedef void  (*deallocator)(void *);
typedef void *(*allocator)(size_t count, size_t size);
typedef void *(*borrower)(void **ptr);

typedef struct s_module
{
	allocator   alloc;
	deallocator dealloc;
	borrower    borrow;

} FnMem;

FnMem *mem_init(void);
void  *mem_borrow(void **from);
void   mem_dispose(FnMem *self);

#endif
