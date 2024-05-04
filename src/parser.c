/* parser.c */
#include "minishell.h"
#include "parser.h"

bool	is_word_token(t_token **token)
{
	return (peek(token, TK_WORD));
}

bool	is_quoted_token(t_token **token)
{
	return (peek(token, TK_SQUOTED_STR) || peek(token, TK_DQUOTED_STR));
}

/**
 * `advance`
 * トークンストリームを次に進める
 */
void	advance(t_token **token)
{
	if (*token && (*token)->type != TK_EOF)
	{
		*token = (*token)->next;
	}
}

/**
 * `skip_blanks`
 * 空白トークンをスキップする
 */
void	skip_blanks(t_token **token)
{
	while (*token && (*token)->type == TK_BLANK)
	{
		advance(token);
	}
}

/**
 * `consume`
 * 現在のトークンが期待しているトークンタイプの時にはそれを消費し
 * トークンストリームを次に進める。空白はスキップされる
 * それ以外のトークンやNULLの場合には偽を返す
 */
bool	consume(t_token **token, t_token_type type)
{
	skip_blanks(token);
	if (!*token || (*token)->type != type)
	{
		return (false);
	}
	advance(token);
	return (true);
}

/**
 * `next_token`
 * トークンストリームから次の有効なトークンを取得し、それを返す。
 * 空白トークンはスキップされる。ポインタは、次のトークンに進む。
 * 次のトークンが存在しない場合は NULL を返す。
 */
t_token	*next_token(t_token **token)
{
	t_token	*current;

	skip_blanks(token);
	if (!*token)
	{
		return (NULL);
	}
	if ((*token)->type == TK_EOF)
	{
		return (*token);
	}
	current = *token;
	advance(token);
	return (current);
}

/**
 * `expect`
 * 指定されたトークンタイプが来た場合にのみトークンを消費
 * 来なかった場合にはエラーを投げる。構文規則の厳格なチェックのために使用
 */
void	expect(t_token **token, t_token_type type)
{
	skip_blanks(token);
	if (!consume(token, type))
	{
		fatal_error("syntax error");
	}
}

/**
 * `peek`
 * 現在のトークンを見る（チェックする）が、消費はしない
 * 先頭のtokenが、指定されたtoken typeと一致するかどうかを返す
 */
bool	peek(t_token **token, t_token_type type)
{
	skip_blanks(token);
	return (*token && (*token)->type == type);
}

t_cmd	*init_execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = calloc(sizeof(*cmd));
	if (!cmd)
	{
		perror_exit("Error: memory allocation failed\n");
	}
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right)
{
	struct s_pipecmd	*cmd;

	cmd = calloc(sizeof(*cmd));
	if (!cmd)
	{
		perror_exit("Error: memory allocation failed\n");
	}
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}

t_redir	*init_redir(t_redir_type type, t_word_list *word, int oflag, int fd)
{
	struct s_redir	*new_redir;

	new_redir = calloc(sizeof(*new_redir));
	if (!new_redir)
	{
		perror_exit("Error: memory allocation failed\n");
	}
	new_redir->type = type;
	new_redir->word_list = word;
	new_redir->oflag = oflag;
	new_redir->fd = fd;
	new_redir->is_here_doc = false;
	new_redir->is_quoted_derimiter = false;
	return (new_redir);
}

bool	is_redir_token(t_token **token)
{
	skip_blanks(token);
	return (*token && (*token)->type != 0 && strchr("<>HA", (*token)->type));
}

t_execcmd	*prepend_redir(t_redir_list **redir_list, t_token **token)
{
	t_redir_list	*new_redir;
	t_word_list		*word_list;
	t_token			*tmp;

	while (is_redir_token(token))
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
				fatal_error("syntax error\n");
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
			fatal_error("syntax error\n");
		}
		// ここでredir_listに追加する処理を書く
		*token = (*token)->next;
	}
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
					fatal_error("syntax error\n");
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
				fatal_error("syntax error\n");
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
}
void	append_word(t_word_list **word_list, char *word)
{
	t_word_list	*new_word;
	t_word_list	*last;

	new_word = calloc(sizeof(*new_word));
	if (!new_word)
	{
		perror_exit("Error: malloc failed\n");
	}
	new_word->word = word;
	new_word->next = NULL;
	if (!*word_list)
	{
		*word_list = new_word;
		return ;
	}
	last = *word_list;
	while (last->next)
	{
		last = last->next;
	}
	last->next = new_word;
}

/**
 * `parse_exec`
 * execコマンドを解析する
 */
t_cmd	*parse_exec(t_token **token)
{
	t_execcmd	*exec_cmd;

	exec_cmd = (t_execcmd *)init_execcmd();
	if (!exec_cmd)
	{
		perror_exit("Error: malloc failed\n");
		return (NULL);
	}
	prepend_redir(exec_cmd, token);
	while (!peek(token, TK_PIPE) && !peek(token, TK_EOF))
	{
		if (!is_word_token(token) && !is_quoted_token(token))
		{
			fatal_error("syntax error\n");
		}
		append_word(&exec_cmd->word_list, (*token)->word);
		advance(token);
		prepend_redir(exec_cmd, token);
	}
	return ((t_cmd *)exec_cmd);
}

t_cmd	*parse_pipe(t_token **token)
{
	t_cmd	*cmd;

	cmd = parse_exec(token);
	if (peek(token, TK_PIPE))
	{
		consume(token, TK_PIPE);
		cmd = init_pipecmd(cmd, parse_pipe(token));
	}
	return (cmd);
}

t_cmd	*parser(t_token **token)
{
	t_cmd	*cmd;

	cmd = parse_pipe(token);
	if (!peek(token, TK_EOF))
	{
		fatal_error("syntax error\n");
		// TODO: clean up for allocated memory
	}
	return (cmd);
}
