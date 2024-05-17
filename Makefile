NAME     = minishell
CC       = cc 
CFLAGS   = $(INCLUDES) -g -fsanitize=address
# CFLAGS   = -Wall -Wextra -Werror $(INCLUDES)
RLDIR    = $(shell brew --prefix readline)
INCLUDES = -I include -I$(RLDIR)/include
LIBS     = -lreadline -L$(RLDIR)/lib
SRCS     = src/main.c src/readline.c src/lexer.c src/executor.c \
src/parser_utils.c src/token_utils.c src/error.c src/expander.c \
src/signal.c src/free.c src/heredoc.c src/parser.c src/redirect.c src/builtin.c src/builtin_exit.c
OBJS     = $(SRCS:%.c=%.o)

all: $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)
clean:
	$(RM) $(OBJS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re test
