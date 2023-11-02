#include "lib/lexer.h"

int 	main(int argc, char **argv)
{
	MemRes 		*mem;
	File 		*file; 
	Scanner 	*scan;
	Lexer		*lexer;
	int		count;
	unique_ptr	ownership;

	count = 1;
	if (argc >= 2)
	{
		mem       = NULL;
		file      = NULL;
		scan      = NULL;
		ownership = NULL;
		mem 	  = mem_init();

		if (!mem)
			fprintf(stderr, "Error : Mem initialization failed\n");

		while (count < argc)
		{
			file = file_init(argv[count], mem);
			if (!file)
				fprintf(stderr, "Error : File initialization failed\n");
			else
				ownership = mem->borrow((void**)&file->file_content);
			if (!ownership)
				fprintf(stderr, "Error : Ownership transfer failed\n");
			else
				scan = scanner_init(file->file_path, ownership, file->content_size, mem);
			if (!scan)
				fprintf(stderr, "Error : Scanner initialization failed\n");
			else
			{
				lexer = lexer_init(scan, mem, file->file_path);
				if (!lexer)
					fprintf(stderr, "Error : Lexer initialization failed\n");
			}
			lexer_dispose(lexer);
			scanner_dispose(scan);
			file_dispose(file);
			lexer 	  = NULL;
			file      = NULL;
			scan      = NULL;
			ownership = NULL;
			++count;
		}
		mem_dispose(mem);
	}
	return (0);
}
