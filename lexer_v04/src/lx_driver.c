#include "../lib/lx_driver.h"
#include <stdio.h>

void driver_print_token(Token *tok)
{
	static char kind[10][6] = {
	    {"<end>"}, {"<err>"}, {"<dir>"}, {"<idt>"}, {"<cmt>"},
	    {"<lit>"}, {"<num>"}, {"<pun>"}, {"<ope>"}, {"<key>"},
	};
	// printf("[ls:%d][cs:%d]", tok->row_start, tok->col_start);
	printf("%s:%.*s", kind[tok->kind], tok->ptr_len, tok->ptr);
	// printf("%s:%s", kind[tok->kind], tok->ptr);
	// printf("[le:%d][ce:%d]\n", tok->row_end, tok->col_end);
}

void driver_start_process(char *path)
{
	FnMem   *mem;
	File    *file;
	Scanner *scan;
	Token   *tok;

	mem = mem_init();
	if (!mem)
		return;
	file = file_init(path, mem);
	if (!file)
		return;
	scan = scanner_init(mem->borrow((void **) &file->content), file->size, mem);
	if (!scan)
		return;

	do
	{
		tok = token_make(scan, mem);
		driver_print_token(tok);
		free(tok);
	} while (scan->cursor < scan->size);

	// write(1, scan->content, scan->size);

	scanner_dispose(scan);
	file_dispose(file);
	mem_dispose(mem);
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		driver_start_process(argv[1]);
	}

	return (0);
}
