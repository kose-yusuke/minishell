#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

// TODO: later to update
typedef enum e_word_flags
{
	WORD_SQUOTE = 1 << 0,
	WORD_DQUOTE = 1 << 1,
	WORD_ESCAPE = 1 << 2,
	WORD_EXPANSION = 1 << 3,
	WORD_RESERVED = 1 << 4,
	WORD_ASSIGNMENT = 1 << 5,
	WORD_SYNTAX_ERROR = 1 << 6
}						t_word_flags;

/* token -> word_list -> argv となる*/
typedef struct s_word_list
{
	char				*word;
	enum e_word_flags	flag;
	struct s_word_list	*next;
}						t_word_list;

// expandがword_listを解決して、単一のwordに変換する
typedef struct s_redir_list
{
	enum e_token_type	type;
	struct s_word_list	*word_list;
	int					fd;
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
	struct s_word_list	*word_list;
	struct s_redir_list	*redir_list;
}						t_execcmd;

typedef struct s_pipecmd
{
	enum e_cmd_type		type;
	struct s_cmd		*left;
	struct s_cmd		*right;
}						t_pipecmd;

#endif
