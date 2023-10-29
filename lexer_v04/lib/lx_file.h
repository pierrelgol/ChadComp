#ifndef LX_FILE_H
#define LX_FILE_H

#include "lx_memory.h"
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define READ_SIZE 4096

typedef struct
{
	short       fd;
	short       open;
	int         size;
	const char *path;
	char       *content;
	FnMem      *mem;
} File;

File *file_init(const char *path, FnMem *mem);
int   file_stat(File *self, const char *path);
int   file_open(File *self);
int   file_close(File *self);
int   file_read(File *self);
File *file_dispose(File *self);

#endif
