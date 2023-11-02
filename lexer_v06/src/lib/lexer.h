#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

/* ---------------------- Driver.c ---------------------- */



/* ------------------------------------------------------ */

/*
	@file : Memory.c

	This file's function are the backbone of the program's
	flexibility, it's an interface that will allow in the
	future to change the allocation strategy of the whole
	program just by changing the functions underneath the
	interface.

	@Types : 

	void* -> unique_ptr : for more explicit memory handling.
	void  -> (*deallocator)(void *); : deallocator currently : free()
	void* -> (*allocator) (size_t count, size_t size); : allocator currently calloc()

	Memres -> composite type containing the allocator, deallocator, and the borrower
		  function pointers.
*/

/* ---------------------- Memory.c ---------------------- */


typedef void* 		unique_ptr;
typedef void  		(*deallocator)(void *);
typedef void*		(*allocator)(size_t count, size_t size);
typedef unique_ptr 	(*borrower)(void **ptr);

typedef struct 	s_memres
{
	allocator   alloc;
	deallocator dealloc;
	borrower    borrow;

} 		MemRes;

MemRes   
*mem_init(void);
unique_ptr
mem_borrow(void **from);
void     
mem_dispose(MemRes *self);


/* ------------------------------------------------------ */

/*
	@File : File.c

	This file's functions are a cleaner and more appropriate
	interface for file management, they also allow for easier
	debugging. Some custom behaviour will allow in the future
	to fine tune the exact behaviour of the functions but the
	interface will remain stable.

	@Type :

	File is a composite type containing different variables
	related to safe file handling.
*/

/* ----------------------- File.c ----------------------- */

#define READ_SIZE 4096

typedef struct	s_file
{
  int         	file_descriptor;
  int         	content_size;
  int         	file_flags;
  char        	*file_content;
  const char  	*file_path;
  MemRes 	*memory_manager;

} 		File;


/*
	@Testing this part of the file interface is still experimental and will
	need thorough testing to make sure it behave as expected.

	@Notes
	If this proves successful i intend to add more bitwise operations for
	storing informations about objects.
*/

#define FILE_IS_OPEN  		0x0000000F
#define FILE_IS_READ	 	0x000000F0
#define FILE_OPEN_FAILED 	0x00000F00
#define FILE_READ_FAILED 	0x000F0000

File  
*file_init(const char *file_path, MemRes *memory_manager);
bool
file_open(File *self);
bool
file_fstat(File *self);
bool
file_read(File *self);
bool
file_close(File *self);
File
*file_reuse(File *self, const char *new_path);
void
file_dispose(File *self);


/* ------------------------------------------------------ */

/*
	@File : Scanner.c

	This files contains all the methods that operates at the lexeme
	level (aka simple unit). Things line eat_characte(), peek_next_character();
	are all very standard for lexers.

	@Type :

	Scanner is a Composite type that take ownership of the memory that it 
	receives. That memory should be the one containing the content of the 
	file that needs to be lexed. It also contains a pointer to the memory_manager.
	for flexibility.

	@Note :

	again scanner_flags will maybe used to store some additional informations
	i'm not sure yet but will see how testing goes with the file system.
*/

/* ---------------------- Scanner.c --------------------- */

typedef struct	s_scanner
{
	int 		scanner_flags;

	int 		current_line;
	int 		current_line_begin;

	int 		current_cursor_position;

	int 		content_size;
	const char 	*content_path;
	char 		*scanner_content;
	MemRes		*memory_manager;
	
}		Scanner;

Scanner 
*scanner_init(const char *path, unique_ptr *content, int content_size, MemRes *memory_manager);
int
scanner_skip(Scanner *self, int (*controller)(int ch));
int 
scanner_rewind_character(Scanner *self);
int 
scanner_eat_character(Scanner *self);
int 
scanner_peek_curr_character(Scanner *self);
int 
scanner_peek_prev_character(Scanner *self);
int 
scanner_peek_next_character(Scanner *self);
void
scanner_dispose(Scanner *self);

/* ---------------------------------------------------- */

/*
	@file : Lexer.c

	Lexer is the file responsible for the main logic of the program.
	I've tried different designs and I think i'm finally getting a feeling
	of where i want to go with the lexer type. It will contain a Scanner
	object, and an interpreter object (WIP) to fill a stack based table
	of Tokens, I'm still debating how I want to do things for the rest of
	the program.

	@Note : lexer.c & future for the parser.

	I need to explore different data structures and models for the final
	form of the lexer and to start building the parser as well, after some
	research there is a lot of different design, but I'm still ensure how
	i want to do it, historically, lexers used to be on demand (that mean,
	the lexer would stream a small part of a file, emit a few tokens to the
	parser and repeat that cycle). However today's machines are basically
	super computers, so in the context of this lexer it would probably be
	very cheap to simply tokenize the whole file at once.

	@idea :
	Maybe the lexer should acquire the memory of the file ? (probably a waste but
	not sure need testing).

	@todo :

	1 - test implementing a circular buffer 	[_]
	1 - test implementing a hash table		[_]
	1 - test implementing a doubly linked list	[_]

	@type :

	a few types are defined for this file :

	mainly : TokenKind which is an enum of the different type of token
	we want the lexer to emit to the parser. There is also the Token type
	which is the main product of the lexer's work, what will potentially
	be emited directly or on demand to the parser.

*/

/* ---------------------- lexer.c --------------------- */

#define CBUFFER_SIZE 16

typedef enum	e_tokenkind
{
	TOK_ERROR = 0,
	TOK_DIRECTIVE,
	TOK_IDENTIFIER,
	TOK_KEYWORD,
	TOK_SINGLL_COMMENT,
	TOK_MULTIL_COMMENT,
	TOK_STR_LITERAL,
	TOK_NUM_LITERAL,
	TOK_PUNCTUATOR,
	TOK_OPERATOR,
	TOK_END,

	TOK_PUN_COMMA,
	TOK_PUN_LCURL,
	TOK_PUN_RCURL,
	TOK_PUN_RPAR,
	TOK_PUN_RBRACK,
	TOK_PUN_SCOLON,
	TOK_PUN_COLON,

	TOK_KW_AUTO,
	TOK_KW_BREAK,
	TOK_KW_CASE,
	TOK_KW_CHAR,
	TOK_KW_CONST,
	TOK_KW_CONTINUE,
	TOK_KW_DEFAULT,
	TOK_KW_DO,
	TOK_KW_DOUBLE,
	TOK_KW_ELSE,
	TOK_KW_ENUM,
	TOK_KW_EXTERN,
	TOK_KW_FLOAT,
	TOK_KW_FOR,
	TOK_KW_GOTO,
	TOK_KW_IF,
	TOK_KW_INT,
	TOK_KW_LONG,
	TOK_KW_REGISTER,
	TOK_KW_RESTRICT,
	TOK_KW_RETURN,
	TOK_KW_SHORT,
	TOK_KW_SIGNED,
	TOK_KW_SIZEOF,
	TOK_KW_STATIC,
	TOK_KW_STRUCT,
	TOK_KW_SWITCH,
	TOK_KW_TYPEDEF,
	TOK_KW_UNION,
	TOK_KW_UNSIGNED,
	TOK_KW_VOID,
	TOK_KW_VOLATILE,
	TOK_KW_WHILE,

	TOK_OP_PLUS,
	TOK_OP_MINUS,
	TOK_OP_MULT,
	TOK_OP_DIV,
	TOK_OP_MOD,
	TOK_OP_EQ,
	TOK_OP_AMPERSAND,
	TOK_OP_OR,
	TOK_OP_XOR,
	TOK_OP_NOT,
	TOK_OP_TILDE,
	TOK_OP_QMARK,
	TOK_OP_COLON,
	TOK_OP_L_PAR,
	TOK_OP_L_BRACK,
	TOK_OP_L_CHEV,
	TOK_OP_R_CHEV,
	TOK_OP_DOT,

	TOK_OP_PLUS_PLUS,
	TOK_OP_MINUS_MINUS,
	TOK_OP_2LCHEV,
	TOK_OP_2RCHEV,
	TOK_OP_PLUS_EQ,
	TOK_OP_MINUS_EQ,
	TOK_OP_STRUCT_PTR,
	TOK_OP_MULT_EQ,
	TOK_OP_DIV_EQ,
	TOK_OP_MOD_EQ,
	TOK_OP_EQ_EQ,
	TOK_OP_AND_EQ,
	TOK_OP_AND_AND,
	TOK_OP_OR_EQ,
	TOK_OP_OR_OR,
	TOK_OP_XOR_EQ,
	TOK_OP_NOT_EQ,
	TOK_OP_TILDE_EQ,
	TOK_OP_LCHEV_EQ,
	TOK_OP_RCHEV_EQ,
	TOK_OP_2LCHEV_EQ,
	TOK_OP_2RCHEV_EQ,

	TOK_OP_TERNARY,
	
}		TokenKind;



typedef struct	s_token
{
	TokenKind 		kind;
	int 					token_flags;

	int		line_start;
	int		col_start;

	int		line_end;
	int		col_end;

	int		ptr_len;
	char		*ptr;
	const char	*path;

}		Token;

typedef struct	s_lexer
{
	Scanner		*scan;
	MemRes		*memory_manager;
	int		buffer_size;
	int		token_count;
	Token		*token_buffer;
	const char 	*path;
	
}Lexer;

Lexer
*lexer_init(Scanner *scanner, MemRes *memory_manager, const char *path);


Token
*lexer_rewind_token(Lexer *self);
Token
*lexer_eat_token(Lexer *self);
Token
*lexer_peek_curr_token(Lexer *self);
Token
*lexer_peek_prev_token(Lexer *self);
Token
*lexer_peek_next_token(Lexer *self);
Token
lexer_produce_token(Lexer *self);
void
lexer_dispose(Lexer *self);

int 
is_newline(int n);
int	
is_whitespace(int n);
int 
is_alpha(int n);
int 
is_alnum(int n);
int 
is_digit(int n);
int
is_operator(int n);
int
is_operator_inside(int n);
TokenKind
is_punctuator_kind(int n);
TokenKind 
is_operator_kind(int n1, int n2, int n3);
int 
is_punctuator(int n);
int 
is_identifier_start(int n);
int 
is_identifier_inside(int n);
int 
is_comment_start(int n1, int n2);
int 
is_single_line_commment_end(int n);
int 
is_multi_line_commment_end(int n1, int n2);
int 
is_directive(int n);
int
is_string_literal_start(int n);
int
is_string_literal_end(int n);
int
is_number_literal_inside(int n);
int
tokncmp(char *s1, char *s2, int n);
TokenKind 
is_keyword(Token *token);

#endif
