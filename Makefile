NAME     = minishell
CC       = cc
CFLAGS   = $(INCLUDES) -g -fsanitize=address
# CFLAGS   = -Wall -Wextra -Werror $(INCLUDES)
RLDIR    = $(shell brew --prefix readline)
INCLUDES = -I include -I$(RLDIR)/include
LIBS     = -lreadline -L$(RLDIR)/lib
SRCS     = src/main.c src/ft_readline.c src/lexer.c src/executor.c \
src/parser_utils.c src/token_utils.c src/error.c  src/expander.c\
src/signal.c src/free.c src/heredoc.c src/parser.c  \
src/builtin.c src/ft_split.c src/env.c \
src/ft_hash.c src/exec_redir.c src/ast.c \
src/builtin_exit.c src/builtin_cd.c src/builtin_env.c \
src/builtin_echo.c src/builtin_export.c src/builtin_pwd.c \
src/builtin_unset.c src/utils.c src/safe_dup.c src/safe_dup2.c \
src/exec_pipe.c
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
