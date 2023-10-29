lCC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -ggdb
NAME = lexer

SDIR = ./src
LDIR = ./lib

SRCS = $(SDIR)/lx_driver.c		\
	   $(SDIR)/lx_file.c 		\
	   $(SDIR)/lx_scanner.c 	\
	   $(SDIR)/lx_memory.c 		\
	   $(SDIR)/lx_token.c 		\
	   
	   
HDRS = $(LDIR)/lx_driver.h 		\
	   $(LDIR)/lx_file.h 		\
	   $(LDIR)/lx_scanner.h 	\
	   $(LDIR)/lx_memory.h 		\
	   $(LDIR)/lx_token.h 		\

OBJS = $(patsubst $(SDIR)/%.c, $(SDIR)/%.o, $(SRCS))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(SDIR)/%.o: $(SDIR)/%.c $(HDRS)
	$(CC) $(CFLAGS) -I$(LDIR) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

test :
	./$(NAME) test_lex.txt

re: fclean all test
