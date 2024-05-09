#ifndef PARSER_H
# define PARSER_H

# include "token.h"

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

/*


	// 仮のコード
	redir = init_redir(REDIR_OUT, NULL, O_WRONLY | O_CREAT, STDOUT_FILENO);
	// tokenを読み解いて、redirを作���する処理を書く
	{
		while (is_redir_token(*token))
		{
			if (peek(token, TK_REDIR_IN))
			{
				redir = init_redir(REDIR_IN, NULL, O_RDONLY, STDIN_FILENO);
				advance(token);
			}
			else if (peek(token, TK_REDIR_OUT))
			{
				redir = init_redir(REDIR_OUT, NULL, O_WRONLY | O_CREAT,
						STDOUT_FILENO);
				advance(token);
			}
			else if (peek(token, TK_REDIR_APPEND))
			{
				redir = init_redir(REDIR_APPEND, NULL,
						O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
				advance(token);
			}
			else if (peek(token, TK_HEREDOC))
			{
				redir = init_redir(REDIR_HEREDOC, NULL, O_RDONLY, STDIN_FILENO);
				advance(token);
				if (!is_word_token(token))
				{
					error_exit("syntax error\n");
				}
				word_list = malloc(sizeof(*word_list));
				if (!word_list)
				{
					perror_exit("Error: malloc failed\n");
				}
				memset(word_list, 0, sizeof(*word_list));
				word_list->word = strdup((*token)->word);
				if (!word_list->word)
				{
					perror_exit("Error: strdup failed\n");
				}
				word_list->next = NULL;
				redir->word_list = word_list;
				redir->is_here_doc = true;
				redir->is_quoted_derimiter = false;
				advance(token);
			}
			else
			{
				error_exit("syntax error\n");
			}
			// ここでredir_listに追加する処理を書く
			*token = (*token)->next;
		}
		// 仮のコード
		redir = init_redir(REDIR_OUT, NULL, O_WRONLY | O_CREAT, STDOUT_FILENO);
		// tokenを読み解いて、redirを作���する処理を書く
		// redir->type = REDIR_OUT;
		// redir->word_list = NULL;
		// redir->oflag = O_WRONLY | O_CREAT;
		// redir->fd = STDOUT_FILENO;
		// redir->is_here_doc = false;
		// redir->is_quoted_derimiter = false;
		// word_list = malloc(sizeof(*word_list));
		// if (!word_list)
		// {
		// 	perror_exit("Error: malloc failed\n");
		// }
		// memset(word_list, 0, sizeof(*word_list));
		// word_list->word = strdup("output.txt");
		// if (!word_list->word)
		// {
		// 	perror_exit("Error: strdup failed\n");
		// }
		// word_list->next = NULL;
		// redir->word_list = word_list;
		if (exec_cmd->redir_list)
		{
			redir->next = exec_cmd->redir_list;
			exec_cmd->redir_list = redir;
		}
		else
		{
			exec_cmd->redir_list = redir;
			redir->next = NULL;
		}
		return (exec_cmd);
	}

 */
