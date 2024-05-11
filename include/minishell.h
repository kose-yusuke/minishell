/* minishell.h - 全体的なプロジェクトのコアファイル */
#ifndef MINISHELL_H
# define MINISHELL_H

// # include "libft.h"
# include "token.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_mgr
{
	int	status;
	int	syntax_error;
}		t_mgr;

/* error.c */
void	perror_exit(const char *msg);
void	error_exit(const char *msg);

/* free.c */
void	free_tokens(t_token *token);

// 以下は引き継いだ部分
// void	fatal_error(const char *msg);
// void	assert_error(const char *msg);
// void	err_exit(const char *location, const char *msg, int status);
// void	todo(const char *msg);
// void	tokenize_error(const char *location, char **rest, char *line);

// // error
// # define ERROR_TOKENIZE 258

// // expand
// void	expand(t_token *tok);

// void	quote_removal(t_token *tok);
// void	append_char(char **s, char c);

#endif
