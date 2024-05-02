#include "minishell.h"
#include "parser.h"

/**
 * `skip_blanks`
 * 空白トークンをスキップする
 */
void	skip_blanks(t_token **token)
{
	while (*token && (*token)->type == TK_BLANK)
	{
		*token = (*token)->next;
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
	*token = (*token)->next;
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
	*token = (*token)->next;
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
bool	peek(t_token **token, t_token_type type, bool should_skip_blanks)
{
	if (should_skip_blanks)
	{
		skip_blanks(token);
	}
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

t_execcmd	*parse_redir(t_execcmd *exec_cmd, t_token **token)
{
	t_redir		*redir;
	t_word_list	*word_list;
	t_token		*tmp;

	while ((peek(token, TK_REDIR_IN) || peek(token, TK_REDIR_OUT) || peek(token,
				TK_REDIR_APPEND)) || peek(token, TK_HEREDOC))
	{
		// ここでword_listに追加する処理を書く
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

bool	is_word_token(t_token **token)
{
	return (peek(token, TK_WORD, true));
}

bool	is_quoted_token(t_token **token)
{
	return (peek(token, TK_SQUOTED_STR, true) || peek(token, TK_DQUOTED_STR, true));
}

bool	is_redirection_token(t_token *token)
{
	return (token && (token->type == TK_REDIR_IN || token->type == TK_REDIR_OUT
			|| token->type == TK_HEREDOC || token->type == TK_REDIR_APPEND));
}

t_cmd	*parse_exec(t_token **token)
{
	t_execcmd	*exec_cmd;

	exec_cmd = (t_execcmd *)init_execcmd();
	if (!exec_cmd)
	{
		perror("Failed to allocate memory for exec command");
		return (NULL);
	}
	parse_redir(exec_cmd, token); //  redir_listへの追加
	while (!peek(token, TK_PIPE, true) && !peek(token, TK_EOF, true))
	{
		if (!peek(token, TK_WORD, true) && !peek(token, TK_SQUOTED_STR, true)
			&& !peek(token, TK_DQUOTED_STR, true))
		{
			fatal_error("syntax error\n");
		}
		if (!is_word_token(*token) && !is_quoted_token(*token))
		{
			fatal_error("syntax error\n");
		}
		append_word(exec_cmd->word_list, (*token)->word);
		*token = (*token)->next;
		parse_redir(exec_cmd, token); //  redir_listへの追加
	}
	return ((t_cmd *)exec_cmd);
}

/*
struct cmd*
parseexec(char **ps, char *es)
{
  struct execcmd *cmd;
  struct cmd *ret;

  char *q, *eq;
  int tok, argc;
  ret = execcmd();
  cmd = (struct execcmd*)ret;
  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|")){
	if((tok=gettoken(ps, es, &q, &eq)) == 0)
		break ;
	if(tok != 'a')
		panic("syntax");
	cmd->argv[argc] = q;
	cmd->eargv[argc] = eq;
	argc++;
	if(argc >= MAXARGS)
		panic("too many args");
	ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return (ret);
}
 */

t_cmd	*parse_pipe(t_token **token)
{
	t_cmd	*cmd;

	cmd = parse_exec(token);
	if (peek(token, TK_PIPE, true))
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
	if (!peek(token, TK_EOF, true))
	{
		fatal_error("syntax error\n");
		// TODO: clean up for allocated memory
	}
	return (cmd);
}
