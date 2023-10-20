#ifndef FILE_H_
#define FILE_H_

#include "common.h"

/* Private File Implementation */

File *__file_init__(const char *path);
bool  __file_open__(File *self);
bool  __file_close__(File *self);
bool  __file_peek__(File *self);
bool  __file_read__(File *self);
File *__file_dispose__(File *self);

/* Public File Interface */

// typedef void* FileHandle;

FileHandle  GetFileHandle(const char *path);
int         GetFileFd(FileHandle file);
int         GetFileSize(FileHandle file);
bool        IsFileValid(FileHandle file);
bool        IsFileOpen(FileHandle file);
const char *GetFilePath(FileHandle file);
char       *GetFileContent(FileHandle file);
void	    ReleaseFileHandle(FileHandle file);

#endif // file.h
