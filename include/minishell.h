/* minishell.h - 全体的なプロジェクトのコアファイル */
#ifndef MINISHELL_H
# define MINISHELL_H

# include "ast.h"
# include "env.h"
# include "ft_hash.h"
# include "token.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_mgr
{
	int							exit_status;
	t_token						*token;
	t_cmd						*cmd;
	t_hash_table				*env_table;
}								t_mgr;

extern volatile sig_atomic_t	g_status;

# include "builtins.h"
# include "error.h"
# include "executor.h"
# include "expander.h"
# include "ft_hash.h"
# include "ft_readline.h"
# include "lexer.h"
# include "parser.h"
# include "redirect.h"
# include "signals.h"
# include "utils.h"
# include <readline/history.h>
# include <readline/readline.h>

// debug
void							print_cmd(t_cmd *cmd);
void							print_redir_list(t_redir *redir_list);
void							print_tokens(t_token *token);
void							print_word_list(t_word *word_list);

/* free.c */
void							free_mgr_resources(t_mgr *mgr);
void							free_argv(char **argv);

// calloc
void							*ft_calloc(size_t count, size_t size);
// itoa
char							*ft_itoa(int n);

/* signal.c */
void							setup_signals(void);

/* safe_*.c */
int								safe_dup(int fildes);
int								safe_dup2(int fildes, int fildes2);

#endif
