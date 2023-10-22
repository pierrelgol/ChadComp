#include "../lib/file.h"

#include <stdio.h>

File *file_init(const char *path)
{
	File *self;

	if (!path)
		return (NULL);
	self = (File *) calloc(1, sizeof(File));
	if (!self)
		return (NULL);
	self->path = path;
	if (!file_stat(self, path))
		return (file_dispose(self));
	if (!(self->content = calloc(self->size + 1, sizeof(char))))
		return (file_dispose(self));
	if (!file_open(self))
		return (file_dispose(self));
	if (!file_read(self))
		return (file_dispose(self));
	return (self);
}

int file_stat(File *self, const char *path)
{
	char *buffer;
	int   byte_read;

	if (!self || !path)
		return (0);
	if (!file_open(self))
		return (0);

	buffer = (char[READ_SIZE]){0};
	byte_read = READ_SIZE;
	while (byte_read == READ_SIZE)
	{
		byte_read = read(self->fd, buffer, READ_SIZE);
		self->size += byte_read;
	}
	if (!file_close(self))
		return (0);
	return (1);
}

int file_open(File *self)
{
	if (!self)
		return (0);
	if ((self->fd = open(self->path, O_RDONLY)) < 0)
		return (0);
	self->open = 1;
	return (1);
}

int file_close(File *self)
{
	if (!self)
		return (0);
	if (self->open)
		self->fd = close(self->fd);
	return (1);
}

int file_read(File *self)
{
	if (!self)
		return (0);
	if (!self->open)
		return (0);
	if ((self->size = read(self->fd, self->content, self->size)) <= 0)
		return (0);
	return (1);
}

File *file_dispose(File *self)
{
	if (self)
	{
		if (self->open)
			file_close(self);
		if (self->content)
			free(self->content);
		free(self);
	}
	return (NULL);
}
