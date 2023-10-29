#ifndef LX_SCANNER_H
#define LX_SCANNER_H

#include "lx_memory.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int    line;
	int    size;
	int    cursor;
	int    begline;
	char  *content;
	FnMem *mem;

} Scanner;

Scanner *scanner_init(char *stream, int stream_size, FnMem *mem);
int      scanner_consume(Scanner *self);
int      scanner_trim(Scanner *self);
int      scanner_next(Scanner *self);
int      scanner_curr(Scanner *self);
int      scanner_prev(Scanner *self);
int      scanner_peek(Scanner *self, int index);
void     scanner_dispose(Scanner *self);

#endif
