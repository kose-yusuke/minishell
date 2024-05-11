#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "token.h"
# include <errno.h>

typedef struct s_word
{
	struct s_token		*token;
	struct s_word		*next;
}						t_word;

// filename または heredocのkeywordのためのtoken
typedef struct s_redir
{
	enum e_token_type	redir_type;
	int					fd;
	struct s_word		*word_list;
	struct s_redir		*next;
}						t_redir;

typedef enum e_cmd_type
{
	NONE,
	EXEC,
	PIPE
}						t_cmd_type;

typedef struct s_cmd
{
	enum e_cmd_type		type;
}						t_cmd;

typedef struct s_execcmd
{
	enum e_cmd_type		type;
	struct s_word		*word_list;
	struct s_redir		*redir_list;
}						t_execcmd;

typedef struct s_pipecmd
{
	enum e_cmd_type		type;
	struct s_cmd		*left;
	struct s_cmd		*right;
}						t_pipecmd;

/* parser_utils.c - パーサー関連のユーティリティ関数 */
void					advance(t_token **token);
void					skip_blanks(t_token **token);
bool					peek(t_token **token, t_token_type type);
bool					consume(t_token **token, t_token_type type);
t_token					*next_token(t_token **token);

#endif
