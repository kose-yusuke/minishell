/* minishell.h - 全体的なプロジェクトのコアファイル */
#ifndef MINISHELL_H
# define MINISHELL_H

// # include "libft.h"
# include "ast.h"
# include "ft_hash.h"
# include "token.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_mgr
{
	int				status;
	bool			syntax_error;
	t_token			*token;
	t_cmd			*cmd;
	t_hash_table	*env_table;
}					t_mgr;

/* error.c */
/* error.c -- Functions for handling errors. */
# include "minishell.h"

// TODO: おそらくdebug用の関数
void				assert_error(const char *msg, char *location);
// TODO: おそらくdebug用の関数
void				todo(const char *msg);
void				error_exit(const char *msg, int exit_status);
void				lexer_error(const char *location, char **rest, char *line);
void				report_error(char *cmd, char *file, char *info);

/* free.c */
void				free_mgr_resources(t_mgr *mgr);

// 以下は引き継いだ部分

// // error
// # define ERROR_TOKENIZE 258

// // expand
// void	expand(t_token *tok);

// void	quote_removal(t_token *tok);
// void	append_char(char **s, char c);

#endif
