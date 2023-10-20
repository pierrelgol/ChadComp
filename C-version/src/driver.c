#include "../lib/scanner.h"
#include "../lib/common.h"
#include "../lib/driver.h"
#include "../lib/lexer.h"
#include "../lib/file.h"
#include <stdio.h>

void	initiate_file_interface(FileInterface *interface)
{
	interface->GetHandleForFile = GetFileHandle;
	interface->DestroyFile = ReleaseFileHandle;
	interface->GetFileFd = GetFileFd;
	interface->GetFilePath = GetFilePath;
	interface->GetFileSize = GetFileSize;
	interface->IsFileOpen = IsFileOpen;
	interface->IsFileValid = IsFileValid;
	interface->AcquireContent = GetFileContent;
}

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		FileInterface interface;
		initiate_file_interface(&interface);

		FileHandle file = GetFileHandle(argv[1]);
		printf("fd      = %d\n",GetFileFd(file));
		printf("size    = %d\n",GetFileSize(file));
		printf("valid   = %d\n",IsFileValid(file));
		printf("open    = %d\n",IsFileOpen(file));
		printf("path    = %s\n",GetFilePath(file));
		ScannerHandle scanner =  GetScannerHandle(file, interface);
		int ch;
		while ((ch = ScannerConsume(scanner)) != EOF)
		{
			printf("%c",ch);
		}
		ReleaseScannerHandle(scanner);
		ReleaseFileHandle(file);
	}

	return (0);
}
