CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
NAME = lexer

SDIR = ./src
LDIR = ./lib

SRCS = $(SDIR)/driver.c		\
	   $(SDIR)/file.c 		\
	   $(SDIR)/lexer.c 		\
	   $(SDIR)/token.c 		\
	   
	   
HDRS = $(LDIR)/driver.h 	\
	   $(LDIR)/file.h 		\
	   $(SDIR)/lexer.h 		\
	   $(SDIR)/token.h 		\

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

re: fclean all test

test : $(NAME)
	./$(NAME) test.c
