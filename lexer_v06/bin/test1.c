#include "../lib/lx_driver.h"
#include <stdio.h>

TokenPrint lookup[1024] = {
    [TOKEN_EOF].str = "EOF",
    [TOKEN_KEYWORD].str = "KEYWORD",
    [TOKEN_IGNORE].str = "IGNORE",
    [TOKEN_DIRECTIVE].str = "DIRECTIVE",
    [TOKEN_IDENTIFIER].str = "IDENTIFIER",
    [TOKEN_SINGLE_LINE_COMMENT].str = "SL_COMMENT",
    [TOKEN_MULTILINE_COMMENT].str = "ML_COMMENT",
    [TOKEN_STRING_LITERAL].str = "STR_LIT",
    [TOKEN_NUMBER_LITERAL].str = "NBR_LIT",
    [TOKEN_SEP_OPEN_CURLY_BRACKETS].str = "O_CRLY_BRK",
    [TOKEN_SEP_CLOSE_CURLY_BRACKETS].str = "C_CRLY_BRK",
    [TOKEN_SEP_SEMI_COLON].str = "S_COLON",
    [TOKEN_SEP_COLON].str = "COLON",
    [TOKEN_SEP_COMMA].str = "COMMA",
    [TOKEN_SEP_SINGLE_QUOTE].str = "S_QUOTE",
    [TOKEN_SEP_DOUBLE_QUOTE].str = "D_QUOTE",
    [TOKEN_OPE_OPEN_SQUARE_BRACKETS].str = "O_SQR_BRK",
    [TOKEN_OPE_CLOSE_SQUARE_BRACKETS].str = "C_SQR_BRK",
    [TOKEN_OPE_OPEN_PARANTHESE].str = "O_PARANT",
    [TOKEN_OPE_CLOSE_PARANTHESE].str = "C_PARANT",
    [TOKEN_OPE_PLUS].str = "PLUS",
    [TOKEN_OPE_MINUS].str = "MINUS",
    [TOKEN_OPE_MULT].str = "MULT",
    [TOKEN_OPE_DIV].str = "DIV",
    [TOKEN_OPE_MOD].str = "MOD",
    [TOKEN_OPE_NOT].str = "NOT",
    [TOKEN_OPE_DOT].str = "DOT",
    [TOKEN_OPE_EQ].str = "EQ",
    [TOKEN_OPE_AND].str = "AND",
    [TOKEN_OPE_BITWISE_TILDE].str = "B_TILDE",
    [TOKEN_OPE_BITWISE_OR].str = "BITWISE_OR",
    [TOKEN_OPE_BITWISE_XOR].str = "BITWISE_XOR",
    [TOKEN_OPE_LESS_THAN].str = "LESS_THAN",
    [TOKEN_OPE_MORE_THAN].str = "MORE_THAN",
    [TOKEN_OPE_SIZEOF].str = "SIZEOF",
    [TOKEN_OPE_STRUCT_PTR].str = "STRUCT_PTR",
    [TOKEN_OPE_PLUS_PLUS].str = "PLUS_PLUS",
    [TOKEN_OPE_MINUS_MINUS].str = "MINUS_MINUS",
    [TOKEN_OPE_PLUS_EQ].str = "PLUS_EQ",
    [TOKEN_OPE_MINUS_EQ].str = "MINUS_EQ",
    [TOKEN_OPE_MULT_EQ].str = "MULT_EQ",
    [TOKEN_OPE_DIV_EQ].str = "DIV_EQ",
    [TOKEN_OPE_MOD_EQ].str = "MOD_EQ",
    [TOKEN_OPE_NOT_EQ].str = "NOT_EQ",
    [TOKEN_OPE_EQ_EQ].str = "EQ_EQ",
    [TOKEN_OPE_BITWISE_TILDE_EQ].str = "B_TILDE_EQ",
    [TOKEN_OPE_BITWISE_AND_EQ].str = "B_AND_EQ",
    [TOKEN_OPE_BITWISE_OR_EQ].str = "B_OR_EQ",
    [TOKEN_OPE_BITWISE_XOR_EQ].str = "XOR_EQ",
    [TOKEN_OPE_LESS_THAN_EQ].str = "LESS_EQ",
    [TOKEN_OPE_MORE_THAN_EQ].str = "MORE_EQ",
    [TOKEN_OPE_BITWISE_LSHIFT].str = "LSHIFT",
    [TOKEN_OPE_BITWISE_RSHIFT].str = "RSHIFT",
    [TOKEN_OPE_LOGICAL_AND].str = "LOGAND",
    [TOKEN_OPE_LOGICAL_OR].str = "LOGOR",
    [TOKEN_KWORD_AUTO].str = "AUTO",
    [TOKEN_KWORD_BREAK].str = "BREAK",
    [TOKEN_KWORD_CASE].str = "CASE",
    [TOKEN_KWORD_CHAR].str = "CHAR",
    [TOKEN_KWORD_CONST].str = "CONST",
    [TOKEN_KWORD_CONTINUE].str = "CONTINUE",
    [TOKEN_KWORD_DEFAULT].str = "DEFAULT",
    [TOKEN_KWORD_DO].str = "DO",
    [TOKEN_KWORD_DOUBLE].str = "DOUBLE",
    [TOKEN_KWORD_ELSE].str = "ELSE",
    [TOKEN_KWORD_ENUM].str = "ENUM",
    [TOKEN_KWORD_EXTERN].str = "EXTERN",
    [TOKEN_KWORD_FLOAT].str = "FLOAT",
    [TOKEN_KWORD_FOR].str = "FOR",
    [TOKEN_KWORD_GOTO].str = "GOTO",
    [TOKEN_KWORD_IF].str = "IF",
    [TOKEN_KWORD_INT].str = "INT",
    [TOKEN_KWORD_LONG].str = "LONG",
    [TOKEN_KWORD_REGISTER].str = "REGISTER",
    [TOKEN_KWORD_RETURN].str = "RETURN",
    [TOKEN_KWORD_SHORT].str = "SHORT",
    [TOKEN_KWORD_SIGNED].str = "SIGNED",
    [TOKEN_KWORD_SIZEOF].str = "SIZEOF",
    [TOKEN_KWORD_STATIC].str = "STATIC",
    [TOKEN_KWORD_STRUCT].str = "STRUCT",
    [TOKEN_KWORD_SWITCH].str = "SWITCH",
    [TOKEN_KWORD_TYPEDEF].str = "TYPEDEF",
    [TOKEN_KWORD_UNION].str = "UNION",
    [TOKEN_KWORD_UNSIGNED].str = "UNSIGNED",
    [TOKEN_KWORD_VOID].str = "VOID",
    [TOKEN_KWORD_VOLATILE].str = "VOLATILE",
    [TOKEN_KWORD_WHILE].str = "WHILE",
    [TOKEN_ERROR].str = "ERROR",
};

void driver_print_token(Token *tok)
{
	if (tok->kind != TOKEN_IGNORE)
	{
		printf("| %-16s | %-16.*s | %-4d | \n", lookup[tok->kind].str, tok->ptr_len, tok->ptr, tok->ptr_len);
	}
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
