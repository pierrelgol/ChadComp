#ifndef COMMON_H_
#define COMMON_H_

#include "fcntl.h"
#include "unistd.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define FILE_READ_SIZE 4096

typedef void *FileHandle;
typedef void *ScannerHandle;

typedef struct
{
	short       fd;
	char        valid;
	char        open;
	int         size;
	const char *path;
	char       *content;

} File;

typedef FileHandle (*FileConstructor)(const char *path);
typedef int (*AccessFileFd)(FileHandle handle);
typedef int (*AccessFileSize)(FileHandle handle);
typedef bool (*AccessFileValid)(FileHandle handle);
typedef bool (*AccessFileOpen)(FileHandle handle);
typedef const char *(*AccessFilePath)(FileHandle handle);
typedef char *(*AcquireFileContent)(FileHandle handle);
typedef void (*FileDestructor)(FileHandle handle);

typedef struct
{
	FileHandle      File;
	FileConstructor GetHandleForFile;
	FileDestructor  DestroyFile;

	AccessFileFd       GetFileFd;
	AccessFileSize     GetFileSize;
	AccessFileOpen     IsFileOpen;
	AccessFileValid    IsFileValid;
	AccessFilePath     GetFilePath;
	AcquireFileContent AcquireContent;

} FileInterface;

typedef struct
{
	int         line;
	int         offset;
	const char *file_path;

} Position;

typedef struct
{
	int   size;
	int   line;
	int   cursor;
	int   begline;
	char *content;

} Scanner;

typedef enum
{
	TOKEN_END = 0,
	TOKEN_PUNCTUATOR,
	TOKEN_IDENTIFIER,
	TOKEN_OPERATOR,
	TOKEN_PREPROC,
	TOKEN_KEYWORD,
	TOKEN_LITERAL,
	TOKEN_SYMBOL,
	TOKEN_NUMBER,
	TOKEN_NEWLINE,
	TOKEN_COMMENT,
	TOKEN_UNKNOWN,

} TokenKind;

typedef struct
{
	TokenKind kind;
	Position  at;
	int       len;
	int       size;
	char     *text;

} Token;


typedef struct
{
	ScannerHandle Scanner;

} ScannerInterface;

typedef struct
{
} Tokenizer;

typedef struct
{
} SymbolTable;

typedef struct
{
	FileInterface    FileManager;
	ScannerInterface ScannerManager;

} LexerDriver;

#endif // common.h
