/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:41:28 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:42:16 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <stdbool.h>
# include <stdlib.h>

typedef enum e_token_type
{
	TK_UNDEF_TOKEN = -1,
	TK_PARSE_ERROR = -2,
	TK_EOF = 0,
	TK_WORD,
	TK_SQUOTE,
	TK_DQUOTE,
	TK_IO_NUM,
	TK_PIPE,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_HEREDOC,
	TK_APPEND,
	TK_BLANK,
	TK_NL
}						t_token_type;

typedef struct s_token
{
	enum e_token_type	type;
	char				*word;
	struct s_token		*next;
}						t_token;

/* token_checks.c */
bool					is_word_token(t_token *token);
bool					is_quoted_token(t_token *token);
bool					is_word_or_quoted_token(t_token *token);
bool					is_io_num_token(t_token *token);
bool					is_redir_token(t_token *token);

/* token_operations.c */
void					advance(t_token **token);
void					skip_blanks(t_token **token);
bool					peek(t_token **token, t_token_type type);
bool					consume(t_token **token, t_token_type type);
t_token					*next_token(t_token **token);

/* lexer.c */
t_token					*new_token(t_token_type type, char **q, char **eq);

/* token_utils.c */
bool					has_delimiter(t_token *token, const char *delimiters);
#endif
