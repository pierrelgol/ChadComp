#include "../lib/driver.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

void *take_ownership(void **owner)
{
	void *new_owner;

	new_owner = (*owner);
	(*owner) = NULL;
	return (new_owner);
}

void put_nbr(int n)
{
	if (n == 0)
		return;
	put_nbr(n / 10);
	write(1, &"0123456789"[n % 10], 1);
}

void put_str(char *str)
{
	write(1, str, strlen(str));
}
void put_strlen(char *str, int len)
{
	write(1, str, len);
}

void visualize_eating(Lexer *lex)
{
	int ch;
	printf("%d ", lex->line);
	while ((ch = lexer_consume(lex)) != EOF)
	{
		write(1, &ch, 1);
		if (lexer_prev(lex) == '\n')
			put_nbr(lex->line);
		usleep(33000);
	}
}

void print_lexer_state(Lexer *lex)
{
	printf("\n");
	printf("Lexer State                 :\n");
	printf("Lexer Line                  : %d\n",lex->line);
	printf("Lexer Cursor                : %d\n",lex->line);
	printf("Lexer Size                  : %d\n",lex->line);
	printf("Lexer BegLine               : %d\n",lex->line);
	printf("Lexer content[lex->begline] : \n%.*s\n",(lex->cursor - lex->begline),&lex->content[lex->begline]);
	printf("Lexer content[lex->cursor]  : \n%.*s\n",10,&lex->content[lex->cursor]);
}

void store_token(Token *tok)
{
	if (!tok)
		return;
	if (tok->kind == TOKEN_IGNORE)
		return;
	printf("[%4d] | ",tok->line);
	printf("[%3d] | ",tok->col);
	printf("[%7d] | ",tok->ptr_len);
	switch (tok->kind)
	{
	case TOKEN_END:
		printf("[    end   ] | ");
		break;
	case TOKEN_IDENTIFIER:
		printf("[identifier] | ");
		break;
	case TOKEN_DIRECTIVE:
		printf("[directive ] | ");
		break;
	case TOKEN_OPERATOR:
		printf("[ operator ] | ");
		break;
	case TOKEN_NUMBER:
		printf("[  number  ] | ");
		break;
	case TOKEN_LITERAL:
		printf("[ literal  ] | ");
		break;
	case TOKEN_PUNCTUATOR:
		printf("[punctuator] | ");
		break;
	case TOKEN_COMMENT:
		printf("[ comment  ] | ");
		break;
	case TOKEN_KEYWORD:
		printf("[ keyword  ] | ");
		break;
	case TOKEN_UNKNOWN:
		printf("[ unknown  ] | ");
		break;
	case TOKEN_IGNORE:
		printf("[ ignore   ] | ");
		break;
	}
	if (tok->ptr_len <= 10)
		printf("[%10.*s]\n",tok->ptr_len, tok->ptr);
	else
		printf("[%10.*s]\n",10, tok->ptr);
	if (tok)
		free(tok);
}

int main(int argc, char **argv)
{
    File *file;
    Lexer *lex;
    void *unique_ptr;
    Token *tok = NULL;

    if (argc == 2)
    {
        file = file_init(argv[1]);
        unique_ptr = take_ownership((void **)(&file->content));
        lex = lexer_init(unique_ptr, file->size);

 	printf("[line] | [col] | [ptr_len] | [Token Kind] | [Token Text]\n");
        clock_t start_time = clock();

        do
        {
            store_token(tok);
            tok = tokenizer_tokenize(lex);

        } while (tok->kind != TOKEN_END);

        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	int bytes_processed = file->size;
        double mpbs = (bytes_processed / elapsed_time) / (1024 * 1024);
        printf("%.2f mbps\n", mpbs);


        print_lexer_state(lex);

        lexer_dispose(lex);
        file_dispose(file);
    }

    return (0);
}

