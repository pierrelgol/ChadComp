#include "../lib/file.h"
#include <stdbool.h>

/*----------------------- Private File Implementation ----------------------*/

File *__file_init__(const char *path)
{
	File *self;

	if (!path)
		return (NULL);
	self = calloc(1, sizeof(File));
	if (!self)
		return (NULL);
	self->path = path;
	if (!__file_peek__(self))
		return (__file_dispose__(self));
	if (!(self->content = calloc(self->size + 1, sizeof(char))))
		return (__file_dispose__(self));
	if (!__file_open__(self))
		return (__file_dispose__(self));
	if (!__file_read__(self))
		return (__file_dispose__(self));
	return (self);
}

bool __file_open__(File *self)
{
	if (!self)
		return (false);
	if ((self->fd = open(self->path, O_RDONLY)) < 0)
	{
		self->open = false;
		self->valid = false;
		return (false);
	}
	else
	{
		self->open = true;
		self->valid = true;
		return (true);
	}
}

bool __file_peek__(File *self)
{
	char *buffer;
	int   bytes_read;

	if (!self)
		return (false);
	buffer = (char[FILE_READ_SIZE]){0};
	bytes_read = FILE_READ_SIZE;
	if (!__file_open__(self))
		return (false);
	while (bytes_read == FILE_READ_SIZE)
	{
		bytes_read = read(self->fd, buffer, FILE_READ_SIZE);
		self->size += bytes_read;
	}
	if (!__file_close__(self))
		return (false);
	return (true);
}

bool __file_close__(File *self)
{
	if (!self)
		return (false);
	if (self->open)
	{
		self->fd = close(self->fd);
		self->open = false;
		return (true);
	}
	return (false);
}

bool __file_read__(File *self)
{
	if (!self)
		return (false);
	if ((self->size = read(self->fd, self->content, self->size)) > 0)
		return (true);
	return (false);
}

File *__file_dispose__(File *self)
{
	if (self)
	{
		if (self->open)
			__file_close__(self);
		if (self->content)
			free(self->content);
		free(self);
	}
	return (NULL);
}

/*---------------------------------------------------------------------------*/





/*-------------------------- Public File Interface --------------------------*/

FileHandle GetFileHandle(const char *path)
{
	return ((FileHandle) __file_init__(path));
}

int GetFileFd(FileHandle file)
{
	if ((File *) file)
		return (((File *) file)->fd);
	return (0);
}

int GetFileSize(FileHandle file)
{
	if ((File *) file)
		return (((File *) file)->size);
	return (0);
}

bool IsFileValid(FileHandle file)
{
	if ((File *) file)
		return (((File *) file)->valid);
	return (0);
}

bool IsFileOpen(FileHandle file)
{
	if ((File *) file)
		return (((File *) file)->open);
	return (0);
}

const char *GetFilePath(FileHandle file)
{
	if ((File *) file)
		return (((File *) file)->path);
	return (NULL);
}

char *GetFileContent(FileHandle file)
{
	char *new_owner;

	if ((File *) file)
	{
		new_owner = ((File *) file)->content;
		((File*)file)->content = NULL;
		((File*)file)->valid = false;
		return (new_owner);
	}
	return (NULL);
}

void ReleaseFileHandle(FileHandle file)
{
	(void) __file_dispose__(file);
}

/*---------------------------------------------------------------------------*/
