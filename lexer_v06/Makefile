lCC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -ggdb
NAME = bin/lexer

SDIR = ./src
LDIR = ./src/lib

SRCS = $(SDIR)/main.c		\
	   $(SDIR)/file.c 		\
	   $(SDIR)/memory.c 	\
	   $(SDIR)/scanner.c 	\
	   $(SDIR)/lexer.c 		\
	   
HDRS = $(LDIR)/lexer.h 		\

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
	./$(NAME) ./bin/test1.c

re: fclean all test
