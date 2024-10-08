NAME     = minishell
CC       = cc
# CFLAGS   = $(INCLUDES) -g -fsanitize=address
CFLAGS   = -Wall -Wextra -Werror $(INCLUDES)
RLDIR    = $(shell brew --prefix readline)
INCLUDES = -I include -I$(RLDIR)/include
LIBS     = -lreadline -L$(RLDIR)/lib

SRCS     = src/main.c src/builtin.c src/builtin_cd.c src/builtin_cd_utils.c \
           src/builtin_echo.c src/builtin_env.c src/builtin_exit.c src/builtin_export.c \
           src/builtin_pwd.c src/builtin_unset.c src/env_list.c \
           src/env_node.c src/error.c src/error_parser.c src/executor.c src/exec_cmd.c \
           src/exec_pipe.c src/exec_pipe_child.c src/exec_redir.c src/expander.c \
           src/exp_get_var.c src/exp_merge_words.c src/exp_word_list.c src/exp_word_split.c \
           src/exp_word_str.c src/free.c src/free_ast.c src/ft_readline.c \
           src/ft_split.c src/ft_strtol.c src/ft_strtol_utils.c src/heredoc.c \
           src/heredoc_file.c src/lexer.c src/parser.c src/parser_init.c \
           src/parser_redir.c src/signal.c src/signal_hooks.c src/token_checks.c \
           src/token_operations.c src/token_utils.c src/xlibc.c \
           src/util_mem.c src/util_str.c src/util_cmp.c src/util_alloc.c \
           src/lexer_utils.c src/util_ctype.c src/env_sort.c

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

.PHONY: all clean fclean re
