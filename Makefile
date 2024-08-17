NAME     = minishell
CC       = cc
# CFLAGS   = $(INCLUDES) -g -fsanitize=address
CFLAGS   = -Wall -Wextra -Werror $(INCLUDES)
CFLAGS   = $(INCLUDES)
RLDIR    = $(shell brew --prefix readline)
INCLUDES = -I include -I$(RLDIR)/include
LIBS     = -lreadline -L$(RLDIR)/lib

SRCS     = src/main.c src/builtin.c src/builtin_cd.c src/builtin_echo.c \
           src/builtin_env.c src/builtin_exit.c src/builtin_export.c src/builtin_pwd.c \
           src/builtin_unset.c src/debug_helpers.c src/env.c src/error.c \
           src/exec_pipe.c src/exec_redir.c src/executor.c src/exp_exit_status.c \
           src/exp_split_word.c src/expander.c src/free.c src/free_ast.c \
           src/ft_hash.c src/ft_itoa.c src/ft_readline.c src/ft_split.c \
           src/ft_strtol.c src/ft_strtol_utils.c src/heredoc.c src/lexer.c \
           src/merge_words.c src/parser.c src/parser_utils.c src/safe_dup.c \
           src/safe_dup2.c src/setup_signal.c src/signal.c src/token_utils.c src/utils.c

OBJS     = $(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# デバッグビルド
debug: CFLAGS += -g -DDEBUG_MODE
debug: re

.PHONY: all clean fclean re
