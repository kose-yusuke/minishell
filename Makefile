NAME     = minishell
CC       = cc
# CFLAGS   = $(INCLUDES) -g -fsanitize=address
CFLAGS   = -Wall -Wextra -Werror $(INCLUDES)
RLDIR    = $(shell brew --prefix readline)
INCLUDES = -I include -I$(RLDIR)/include
LIBS     = -lreadline -L$(RLDIR)/lib

SRCS     = src/main.c src/builtin.c src/builtin_cd.c src/builtin_echo.c \
           src/builtin_env.c src/builtin_exit.c src/builtin_export.c src/builtin_pwd.c \
           src/builtin_unset.c src/debug_helpers.c src/env.c src/error.c \
           src/exec_pipe.c src/exec_redir.c src/exec_cmd.c src/executor.c \
           src/exp_word_split.c src/expander.c src/free.c src/free_ast.c \
           src/ft_itoa.c src/ft_readline.c src/ft_split.c \
           src/ft_strtol.c src/ft_strtol_utils.c src/heredoc.c src/lexer.c \
           src/exp_merge_words.c src/exp_word_str.c src/exp_get_var.c src/parser.c \
           src/parser_init.c src/signal.c src/signal_hooks.c src/token_checks.c \
           src/token_operations.c src/utils.c src/xlibc.c src/heredoc_file.c \
           src/exec_pipe_child.c src/token_utils.c src/builtin_cd_utils.c \
           src/exp_word_list.c

OBJS     = $(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# デバッグビルド
debug: CFLAGS += -g -DDEBUG_MODE
debug: re

.PHONY: all clean fclean re debug
