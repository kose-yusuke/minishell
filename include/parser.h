#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

typedef struct s_word
{
	enum e_token_type	word_type;
	char				*word;
	bool				connected_to_next;
	struct s_word		*next;
}						t_word;

// filename または heredocのkeywordのためのtoken
typedef struct s_redir_list
{
	enum e_token_type	redir_type;
	int					fd;
	struct s_word		*word;
	struct s_redir_list	*next;
}						t_redir_list;

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
	struct s_token		*token;
	struct s_redir_list	*redir_list;
}						t_execcmd;

typedef struct s_pipecmd
{
	enum e_cmd_type		type;
	struct s_cmd		*left;
	struct s_cmd		*right;
}						t_pipecmd;

#endif
