#ifndef LX_TOKEN_H
#define LX_TOKEN_H

#include "lx_memory.h"
#include "lx_scanner.h"

// typedef enum
// {
// 	LEXEME_RAW_NULL = 0,
// 	LEXEME_RAW_START_OF_HEADING,
// 	LEXEME_RAW_START_OF_TEXT,
// 	LEXEME_RAW_END_OF_TEXT,
// 	LEXEME_RAW_ENQUIRY,
// 	LEXEME_RAW_ACKNOWLEDGE,
// 	LEXEME_RAW_BELL,
// 	LEXEME_RAW_BACKSPACE,
// 	LEXEME_RAW_HORIZONTAL_TAB,
// 	LEXEME_RAW_LINE_FEED,
// 	LEXEME_RAW_VERTICAL_TAB,
// 	LEXEME_RAW_FORM_FEED,
// 	LEXEME_RAW_CARIAGE_RETURN,
// 	LEXEME_RAW_SHIFT_OUT,
// 	LEXEME_RAW_SHIFT_IN,
// 	LEXEME_RAW_DATA_LINK_ESCAPE,
// 	LEXEME_RAW_DEVICE_CONTROL1,
// 	LEXEME_RAW_DEVICE_CONTROL2,
// 	LEXEME_RAW_DEVICE_CONTROL3,
// 	LEXEME_RAW_DEVICE_CONTROL4,
// 	LEXEME_RAW_NEGATIVE_ACKNOWLEDGE,
// 	LEXEME_RAW_SYNCHRONOUS_IDLE,
// 	LEXEME_RAW_END_OF_TRANS_BLOCK,
// 	LEXEME_RAW_CANCEL,
// 	LEXEME_RAW_END_OF_MEDIUM,
// 	LEXEME_RAW_SUBSTITUTE,
// 	LEXEME_RAW_ESCAPE,
// 	LEXEME_RAW_FILE_SEPARATOR,
// 	LEXEME_RAW_GROUP_SEPARATOR,
// 	LEXEME_RAW_RECORD_SEPARATOR,
// 	LEXEME_RAW_UNIT_SEPARATOR,

// 	LEXEME_RAW_SPACE,
// 	LEXEME_RAW_EXCLAMATION_MARK,
// 	LEXEME_RAW_DOUBLE_QUOTES,
// 	LEXEME_RAW_HASHTAG,
// 	LEXEME_RAW_DOLLAR,
// 	LEXEME_RAW_MODULO,
// 	LEXEME_RAW_AMPERSAND,
// 	LEXEME_RAW_SINGLE_QUOTE,
// 	LEXEME_RAW_OPEN_PARANTHESE,
// 	LEXEME_RAW_CLOSE_PARANTHESE,
// 	LEXEME_RAW_PLUS,
// 	LEXEME_RAW_COMMA,
// 	LEXEME_RAW_MINUS,
// 	LEXEME_RAW_SLASH,
// 	LEXEME_RAW_DIGIT_0,
// 	LEXEME_RAW_DIGIT_1,
// 	LEXEME_RAW_DIGIT_2,
// 	LEXEME_RAW_DIGIT_3,
// 	LEXEME_RAW_DIGIT_4,
// 	LEXEME_RAW_DIGIT_5,
// 	LEXEME_RAW_DIGIT_6,
// 	LEXEME_RAW_DIGIT_7,
// 	LEXEME_RAW_DIGIT_8,
// 	LEXEME_RAW_DIGIT_9,
// 	LEXEME_RAW_COLON,
// 	LEXEME_RAW_SEMI_COLON,
// 	LEXEME_RAW_LEFT_CHEVRON,
// 	LEXEME_RAW_EQUAL_SIGN,
// 	LEXEME_RAW_RIGHT_CHEVRON,
// 	LEXEME_RAW_QUESTIONMARK,
// 	LEXEME_RAW_AT,
// 	LEXEME_RAW_CAP_A,
// 	LEXEME_RAW_CAP_B,
// 	LEXEME_RAW_CAP_C,
// 	LEXEME_RAW_CAP_D,
// 	LEXEME_RAW_CAP_E,
// 	LEXEME_RAW_CAP_F,
// 	LEXEME_RAW_CAP_G,
// 	LEXEME_RAW_CAP_H,
// 	LEXEME_RAW_CAP_I,
// 	LEXEME_RAW_CAP_J,
// 	LEXEME_RAW_CAP_K,
// 	LEXEME_RAW_CAP_L,
// 	LEXEME_RAW_CAP_M,
// 	LEXEME_RAW_CAP_N,
// 	LEXEME_RAW_CAP_O,
// 	LEXEME_RAW_CAP_P,
// 	LEXEME_RAW_CAP_Q,
// 	LEXEME_RAW_CAP_R,
// 	LEXEME_RAW_CAP_S,
// 	LEXEME_RAW_CAP_T,
// 	LEXEME_RAW_CAP_U,
// 	LEXEME_RAW_CAP_V,
// 	LEXEME_RAW_CAP_W,
// 	LEXEME_RAW_CAP_X,
// 	LEXEME_RAW_CAP_Y,
// 	LEXEME_RAW_CAP_Z,
// 	LEXEME_RAW_OPEN_ANGLE_BRACKET,
// 	LEXEME_RAW_BACKSLASH,
// 	LEXEME_RAW_CLOSE_ANGLE_BRACKET,
// 	LEXEME_RAW_CARET,
// 	LEXEME_RAW_UNDERSCORE,
// 	LEXEME_RAW_GRAVE_ACCENT,
// 	LEXEME_RAW_MIN_A,
// 	LEXEME_RAW_MIN_B,
// 	LEXEME_RAW_MIN_C,
// 	LEXEME_RAW_MIN_D,
// 	LEXEME_RAW_MIN_E,
// 	LEXEME_RAW_MIN_F,
// 	LEXEME_RAW_MIN_G,
// 	LEXEME_RAW_MIN_H,
// 	LEXEME_RAW_MIN_I,
// 	LEXEME_RAW_MIN_J,
// 	LEXEME_RAW_MIN_K,
// 	LEXEME_RAW_MIN_L,
// 	LEXEME_RAW_MIN_M,
// 	LEXEME_RAW_MIN_N,
// 	LEXEME_RAW_MIN_O,
// 	LEXEME_RAW_MIN_P,
// 	LEXEME_RAW_MIN_Q,
// 	LEXEME_RAW_MIN_R,
// 	LEXEME_RAW_MIN_S,
// 	LEXEME_RAW_MIN_T,
// 	LEXEME_RAW_MIN_U,
// 	LEXEME_RAW_MIN_V,
// 	LEXEME_RAW_MIN_W,
// 	LEXEME_RAW_MIN_X,
// 	LEXEME_RAW_MIN_Y,
// 	LEXEME_RAW_MIN_Z,
// 	LEXEME_RAW_OPEN_CURLY_BRACKET,
// 	LEXEME_RAW_VERTICAL_BAR,
// 	LEXEME_RAW_CLOSE_CURLY_BRACKET,
// 	LEXEME_RAW_TOLDE,

// 	LEXEME_RAW_DEL,

// } LexemeRaw;

typedef enum
{
	LEXEME_LOOKS_EOF = 0,
	LEXEME_LOOKS_IGNOREABLE,
	LEXEME_LOOKS_DIRECTIVE,
	LEXEME_LOOKS_COMMENT,
	LEXEME_LOOKS_IDENTIFIER,
	LEXEME_LOOKS_STRING_LITERAL,
	LEXEME_LOOKS_NUMBER_LITERAL,
	LEXEME_LOOKS_OPERATOR,
	LEXEME_LOOKS_PUNCTUATOR,
	LEXEME_LOOKS_UNRESOLVED,

} LexemeLooks;

typedef enum
{
	TOKEN_EOF = 0,
	TOKEN_KEYWORD = 255,
	TOKEN_IGNORE,
	TOKEN_DIRECTIVE,
	TOKEN_IDENTIFIER,
	TOKEN_SINGLE_LINE_COMMENT,
	TOKEN_MULTILINE_COMMENT,
	TOKEN_STRING_LITERAL,
	TOKEN_NUMBER_LITERAL,

	TOKEN_SEP_OPEN_CURLY_BRACKETS,
	TOKEN_SEP_CLOSE_CURLY_BRACKETS,
	TOKEN_SEP_SEMI_COLON,
	TOKEN_SEP_COLON,
	TOKEN_SEP_COMMA,
	TOKEN_SEP_SINGLE_QUOTE,
	TOKEN_SEP_DOUBLE_QUOTE,

	TOKEN_OPE_OPEN_SQUARE_BRACKETS,
	TOKEN_OPE_CLOSE_SQUARE_BRACKETS,
	TOKEN_OPE_OPEN_PARANTHESE,
	TOKEN_OPE_CLOSE_PARANTHESE,

	TOKEN_OPE_PLUS,
	TOKEN_OPE_MINUS,
	TOKEN_OPE_MULT,
	TOKEN_OPE_DIV,
	TOKEN_OPE_MOD,
	TOKEN_OPE_NOT,
	TOKEN_OPE_DOT,
	TOKEN_OPE_EQ,
	TOKEN_OPE_AND,
	TOKEN_OPE_BITWISE_TILDE,
	TOKEN_OPE_BITWISE_OR,
	TOKEN_OPE_BITWISE_XOR,
	TOKEN_OPE_LESS_THAN,
	TOKEN_OPE_MORE_THAN,
	TOKEN_OPE_SIZEOF,

	TOKEN_OPE_STRUCT_PTR,
	TOKEN_OPE_PLUS_PLUS,
	TOKEN_OPE_MINUS_MINUS,
	TOKEN_OPE_PLUS_EQ,
	TOKEN_OPE_MINUS_EQ,
	TOKEN_OPE_MULT_EQ,
	TOKEN_OPE_DIV_EQ,
	TOKEN_OPE_MOD_EQ,
	TOKEN_OPE_NOT_EQ,
	TOKEN_OPE_EQ_EQ,
	TOKEN_OPE_BITWISE_TILDE_EQ,
	TOKEN_OPE_BITWISE_AND_EQ,
	TOKEN_OPE_BITWISE_OR_EQ,
	TOKEN_OPE_BITWISE_XOR_EQ,
	TOKEN_OPE_LESS_THAN_EQ,
	TOKEN_OPE_MORE_THAN_EQ,
	TOKEN_OPE_BITWISE_LSHIFT,
	TOKEN_OPE_BITWISE_RSHIFT,
	TOKEN_OPE_LOGICAL_AND,
	TOKEN_OPE_LOGICAL_OR,

	TOKEN_KWORD_AUTO,
	TOKEN_KWORD_BREAK,
	TOKEN_KWORD_CASE,
	TOKEN_KWORD_CHAR,
	TOKEN_KWORD_CONST,
	TOKEN_KWORD_CONTINUE,
	TOKEN_KWORD_DEFAULT,
	TOKEN_KWORD_DO,
	TOKEN_KWORD_DOUBLE,
	TOKEN_KWORD_ELSE,
	TOKEN_KWORD_ENUM,
	TOKEN_KWORD_EXTERN,
	TOKEN_KWORD_FLOAT,
	TOKEN_KWORD_FOR,
	TOKEN_KWORD_GOTO,
	TOKEN_KWORD_IF,
	TOKEN_KWORD_INT,
	TOKEN_KWORD_LONG,
	TOKEN_KWORD_REGISTER,
	TOKEN_KWORD_RETURN,
	TOKEN_KWORD_SHORT,
	TOKEN_KWORD_SIGNED,
	TOKEN_KWORD_SIZEOF,
	TOKEN_KWORD_STATIC,
	TOKEN_KWORD_STRUCT,
	TOKEN_KWORD_SWITCH,
	TOKEN_KWORD_TYPEDEF,
	TOKEN_KWORD_UNION,
	TOKEN_KWORD_UNSIGNED,
	TOKEN_KWORD_VOID,
	TOKEN_KWORD_VOLATILE,
	TOKEN_KWORD_WHILE,

	TOKEN_ERROR,

} TokenIdentity;

typedef struct
{
	TokenIdentity kind;
	int           row_start;
	int           row_end;
	int           col_start;
	int           col_end;
	int           kind_size;
	int           ptr_len;
	char         *ptr;

} Token;

Token *token_make(Scanner *scan, FnMem *mem);

// first pass

int         tokncmp(char *token1, char *token2, int n);
LexemeLooks lexeme_looks(Token *token, Scanner *scan);

int is_ascii(int ch);
int is_alpha(int ch);
int is_digit(int ch);
int is_alnum(int ch);
int is_space(int ch);
int is_newln(int ch);
int lexeme_looks_ignoreable(int ch);
int lexeme_looks_directive(int ch);
int lexeme_looks_identifier(int ch);
int lexeme_looks_comment(int ch1, int ch2);
int lexeme_looks_number_literal(int ch);
int lexeme_looks_string_literal(int ch);
int lexeme_looks_operator(int ch);
int lexeme_looks_punctuator(int ch);
int is_identifier(Token *token);
int is_punctuator(int ch);
int is_operator(Token *token);
int is_keyword(Token *token);
int is_comment_start(int ch1, int ch2);
int is_single_comment(int ch1, int ch2);
int is_multi_comment(int ch1, int ch2);

int           is_error(int ch);
TokenIdentity token_identity(Token *token, Scanner *scan, LexemeLooks looks);

#endif
