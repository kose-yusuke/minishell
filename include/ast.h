/* ast.h - 抽象構文木に関する構造体の定義。 */
#ifndef AST_H
# define AST_H

# include "token.h"

typedef struct s_word
{
	struct s_token		*token;
	struct s_word		*next;
}						t_word;

// filename または heredocのkeywordのためのword_list
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

/*
ASTの操作に特化した関数（例えば、ASTノードの追加、削除、検索など）が存在する場合は、それらを ast_utils に分類

トークンの操作に関連する関数は parser_utils に留める
 */
#endif
