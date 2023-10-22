#ifndef FILE_H
#define FILE_H

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

} File;

File *file_init(const char *path);
int   file_stat(File *self, const char *path);
int   file_open(File *self);
int   file_close(File *self);
int   file_read(File *self);
File *file_dispose(File *self);

#endif
