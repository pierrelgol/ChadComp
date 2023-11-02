#include "lib/lexer.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"

char *token_to_str(Token *token)
{
	switch (token->kind) {
        case TOK_END           : return ("end");
        case TOK_DIRECTIVE     : return ("directive");
        case TOK_IDENTIFIER    : return ("identifier");
        case TOK_KEYWORD       : return ("keyword");
        case TOK_SINGLL_COMMENT: return ("slcomment");
        case TOK_MULTIL_COMMENT: return ("mlcomment");
        case TOK_STR_LITERAL   : return ("str_lit");
        case TOK_NUM_LITERAL   : return ("num_lit");
        case TOK_PUNCTUATOR    : return ("punctuator");
        case TOK_OPERATOR      : return ("operator");
	case TOK_PUN_LCURL     : return ("lcurl");
	case TOK_PUN_RCURL     : return ("rcurl");
	case TOK_PUN_RPAR      : return ("rpar");
	case TOK_PUN_RBRACK    : return ("rbrack");
	case TOK_PUN_SCOLON    : return ("scolon");
	case TOK_PUN_COMMA     : return ("comma");
	case TOK_PUN_COLON     : return ("colon");
        case TOK_ERROR         : return (RED"error"RESET);
        case TOK_KW_AUTO       : return ("auto");
        case TOK_KW_BREAK      : return ("break");
        case TOK_KW_CASE       : return ("case");
        case TOK_KW_CHAR       : return ("char");
        case TOK_KW_CONST      : return ("const");
        case TOK_KW_CONTINUE   : return ("continue");
        case TOK_KW_DEFAULT    : return ("default");
        case TOK_KW_DO         : return ("do");
        case TOK_KW_DOUBLE     : return ("double");
        case TOK_KW_ELSE       : return ("else");
        case TOK_KW_ENUM       : return ("enum");
        case TOK_KW_EXTERN     : return ("extern");
        case TOK_KW_FLOAT      : return ("float");
        case TOK_KW_FOR        : return ("for");
        case TOK_KW_GOTO       : return ("goto");
        case TOK_KW_IF         : return ("if");
        case TOK_KW_INT        : return ("int");
        case TOK_KW_LONG       : return ("long");
        case TOK_KW_REGISTER   : return ("register");
        case TOK_KW_RESTRICT   : return ("restrict");
        case TOK_KW_RETURN     : return ("return");
        case TOK_KW_SHORT      : return ("short");
        case TOK_KW_SIGNED     : return ("signed");
        case TOK_KW_SIZEOF     : return ("sizeof");
        case TOK_KW_STATIC     : return ("static");
        case TOK_KW_STRUCT     : return ("struct");
        case TOK_KW_SWITCH     : return ("switch");
        case TOK_KW_TYPEDEF    : return ("typedef");
        case TOK_KW_UNION      : return ("union");
        case TOK_KW_UNSIGNED   : return ("unsigned");
        case TOK_KW_VOID       : return ("void");
        case TOK_KW_VOLATILE   : return ("volatile");
        case TOK_KW_WHILE      : return ("while");
        case TOK_OP_PLUS       : return ("plus");
        case TOK_OP_MINUS      : return ("minus");
        case TOK_OP_MULT       : return ("mult");
        case TOK_OP_DIV        : return ("div");
        case TOK_OP_MOD        : return ("mod");
        case TOK_OP_EQ         : return ("eq");
        case TOK_OP_AMPERSAND  : return ("ampersand");
        case TOK_OP_OR         : return ("or");
        case TOK_OP_XOR        : return ("xor");
        case TOK_OP_NOT        : return ("not");
        case TOK_OP_TILDE      : return ("tilde");
        case TOK_OP_QMARK      : return ("qmark");
        case TOK_OP_COLON      : return ("colon");
        // case TOK_OP_L_PAR      : return ("l_par");
        // case TOK_OP_R_PAR      : return ("r_par");
        // case TOK_OP_L_BRACK    : return ("l_brack");
        // case TOK_OP_R_BRACK    : return ("r_brack");
        case TOK_OP_L_CHEV     : return ("l_chev");
        case TOK_OP_R_CHEV     : return ("r_chev");
        case TOK_OP_DOT        : return ("dot");
        case TOK_OP_PLUS_PLUS  : return ("plus_plus");
        case TOK_OP_MINUS_MINUS: return ("minus_minus");
        case TOK_OP_2LCHEV     : return ("2lchev");
        case TOK_OP_2RCHEV     : return ("2rchev");
        case TOK_OP_PLUS_EQ    : return ("plus_eq");
        case TOK_OP_MINUS_EQ   : return ("minus_eq");
        case TOK_OP_STRUCT_PTR : return ("struct_ptr");
        case TOK_OP_MULT_EQ    : return ("mult_eq");
        case TOK_OP_DIV_EQ     : return ("div_eq");
        case TOK_OP_MOD_EQ     : return ("mod_eq");
        case TOK_OP_EQ_EQ      : return ("eq_eq");
        case TOK_OP_AND_EQ     : return ("and_eq");
        case TOK_OP_AND_AND    : return ("and_and");
        case TOK_OP_OR_EQ      : return ("or_eq");
        case TOK_OP_OR_OR      : return ("or_or");
        case TOK_OP_XOR_EQ     : return ("xor_eq");
        case TOK_OP_NOT_EQ     : return ("not_eq");
        case TOK_OP_TILDE_EQ   : return ("tilde_eq");
        case TOK_OP_LCHEV_EQ   : return ("lchev_eq");
        case TOK_OP_RCHEV_EQ   : return ("rchev_eq");
        case TOK_OP_2LCHEV_EQ  : return ("2lchev_eq");
        case TOK_OP_2RCHEV_EQ  : return ("2rchev_eq");
        case TOK_OP_TERNARY    : return ("ternary");
	default		       : return ("error");
        }
}

void	print_token(Token *token)
{
	// printf(":%2d",token->token_flags);
	// printf(":%2d",token->line_start);
	// printf(":%2d",token->line_end);
	// printf(":%2d",token->col_start);
	// printf(":%2d",token->col_end);
	// printf(":%2d",token->kind);
	// printf(":%2d",token->ptr_len);
	printf(":"RED"%10s"RESET":",token_to_str(token));
	printf(GREEN"%-80.*s"RESET":\n",token->ptr_len,token->ptr);
}

int 	main(int argc, char **argv)
{
	MemRes 		*mem;
	File 		*file; 
	Scanner 	*scan;
	Lexer		*lexer;
	unique_ptr	ownership;
	Token 		current;

	if (argc >= 2)
	{
		mem 	  = mem_init();
		if (!mem)
			return fprintf(stderr, "Error : Mem initialization failed\n");

		file = file_init(argv[1], mem);
		if (!file)
		{
			mem_dispose(mem);
			return fprintf(stderr, "Error : File initialization failed\n");
		}
		ownership = mem->borrow((void**)&file->file_content);
		if (!ownership)
		{
			file_dispose(file);
			mem_dispose(mem);
			return fprintf(stderr, "Error : Ownership transfer failed\n");
		}
		scan = scanner_init(file->file_path, ownership, file->content_size, mem);
		if (!scan)
		{
			file_dispose(file);
			mem_dispose(mem);
			return fprintf(stderr, "Error : Scanner initialization failed\n");
		}
		else
		{
			lexer = lexer_init(scan, mem, file->file_path);
			if (!lexer)
			{
				file_dispose(file);
				scanner_dispose(scan);
				mem_dispose(mem);
				return fprintf(stderr, "Error : Lexer initialization failed\n");
			}
			while ((current = lexer_produce_token(lexer)).kind != TOK_END)
			{
				print_token(&current);
			}
		}
		lexer_dispose(lexer);
		scanner_dispose(scan);
		file_dispose(file);
		mem_dispose(mem);
	}
	return (0);
}
