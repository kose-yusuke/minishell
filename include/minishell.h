/* minishell.h - 全体的なプロジェクトのコアファイル */
#ifndef MINISHELL_H
# define MINISHELL_H

# include "ast.h"
# include "ft_hash.h"
# include "token.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <limits.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <ctype.h>
#include <stddef.h>

# include "env.h"
# include "token.h"
# include "ast.h"

typedef struct s_mgr
{
	int				status;
	bool			syntax_error;
	t_token			*token;
	t_cmd			*cmd;
	t_hash_table	*env_table;
}					t_mgr;

# include "builtins.h"
#include "executor.h"
#include "expander.h"
# include "ft_hash.h"
#include "ft_readline.h"
# include "lexer.h"
# include "parser.h"
# include "redirect.h"
# include "utils.h"

// TODO: おそらくdebug用の関数
void				assert_error(const char *msg, char *location);
// TODO: おそらくdebug用の関数
void				todo(const char *msg);
void				error_exit(const char *msg, int exit_status);
void				lexer_error(const char *location, char **rest, char *line);
void				report_error(char *cmd, char *file, char *info);

// debug
void print_cmd(t_cmd *cmd);
void print_redir_list(t_redir *redir_list);
void	print_tokens(t_token *token);

/* free.c */
void				free_mgr_resources(t_mgr *mgr);

// calloc
void	*ft_calloc(size_t count, size_t size);
/* signal.c */
void setup_signals(void);

#endif
