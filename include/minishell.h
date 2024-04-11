#ifndef MINISHELL_H
# define MINISHELL_H
// # include "libft.h"
# include <stdio.h>
# include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include "lexer.h"
#include "parser.h"

//error
# define ERROR_TOKENIZE 258



void	fatal_error(const char *msg);
void	assert_error(const char *msg);
void	err_exit(const char *location, const char *msg, int status);
void	todo(const char *msg);

// destructor
void	free_tok(t_token *tok);
void	free_argv(char **argv);

// expand
void expand(t_token *tok);;
void	quote_removal(t_token *tok);
void append_char(char **s, char c);

#endif
