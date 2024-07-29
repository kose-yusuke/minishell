/* ast.h - 抽象構文木に関する構造体の定義。 */
#ifndef AST_H
# define AST_H

# include "token.h"

typedef struct s_word
{
	struct s_token		*token;
	struct s_word		*next;
}						t_word;

typedef struct s_redir
{
	enum e_token_type	redir_type;
	int					fd;
	int backup_fd;            // リダイレクト前のfdを保持
	struct s_word *word_list; // TODO: このexpand処理->統合を確認
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
	char				*eof_word;
}						t_execcmd;

typedef struct s_pipecmd
{
	enum e_cmd_type		type;
	struct s_cmd		*left;
	struct s_cmd		*right;
}						t_pipecmd;

void					free_word(t_word *word);
void					free_redir(t_redir *redir);
void					free_cmd(t_cmd *cmd);
void					free_rignt_cmd(t_cmd *cmd);

#endif
