#include "lib/lexer.h"
#include "stdbool.h"
#include <stdio.h>

bool	iswspc(int n)
{
	switch (n) {
        case 9 ... 13 : return (true);
        case 32       : return (true);
	default       : return (false);
        }
}

bool	is_tab(int n)
{
	return (n == '\t');
}

int 	main(int argc, char **argv)
{
	MemRes 		*mem;
	File 		*file; 
	Scanner 	*scan;
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
				int ch;
				while ((ch = scanner_peek_curr_character(scan)) != EOF)
				{
					if (is_tab(ch))
						scanner_skip(scan, is_tab);	
					ch = scanner_peek_curr_character(scan);
					printf("%c",ch);
					scanner_eat_character(scan);
				}
			}
			scanner_dispose(scan);
			file_dispose(file);
			file      = NULL;
			scan      = NULL;
			ownership = NULL;
			++count;
		}
		mem_dispose(mem);
	}
	return (0);
}
