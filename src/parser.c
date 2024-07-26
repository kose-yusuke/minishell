/* parser.c - パーサー関連の関数 */
#include "parser.h"

static t_cmd	*init_execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		report_error("init_execcmd", NULL, "memory allocation failed");
		return (NULL);
	}
	cmd->type = EXEC;
	cmd->word_list = NULL;
	cmd->redir_list = NULL;
	return ((t_cmd *)cmd);
}

static t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right)
{
	struct s_pipecmd	*cmd;

	// TODO: leftとrightがNULLだったらどうする？
	cmd = calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		report_error("init_pipecmd", NULL, "memory allocation failed");
		free_cmd(left);
		free_cmd(right);
		return (NULL);
	}
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

static t_redir	*init_redir(t_token_type type, int fd)
{
	struct s_redir	*new_redir;

	new_redir = calloc(1, sizeof(*new_redir));
	if (!new_redir)
	{
		report_error("init_redir", NULL, "memory allocation failed");
		return (NULL);
	}
	new_redir->redir_type = type;
	new_redir->fd = fd;
	new_redir->backup_fd = -1;
	new_redir->word_list = NULL;
	new_redir->next = NULL;
	return (new_redir);
}

static void	free_word_list(t_word *word_list)
{
	t_word	*current;
	t_word	*next;

	current = word_list;
	while (current)
	{
		next = current->next;
		if (current->token)
		{
			free_tokens(current->token);
		}
		free(current);
		current = next;
	}
}

static void	append_word(t_word **word_list, t_token *token)
{
	t_word	*new_word;
	t_word	*current;

	new_word = calloc(1, sizeof(*new_word));
	if (!new_word)
	{
		report_error("append_word", NULL, "memory allocation failed");
		free_word_list(*word_list);
		return ;
	}
	new_word->token = token;
	new_word->next = NULL;
	if (!*word_list)
	{
		*word_list = new_word;
		return ;
	}
	current = *word_list;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new_word;
}

static void	process_redir_words(t_word **word_list, t_token **token)
{
	if (!is_word_or_quoted_token(*token))
	{
		// bash: syntax error near unexpected token 'newline' <- 本来のError message
		report_error("parse_redir", NULL, "syntax error(1)");
		// XXX: この構文エラーは、ここでparseをやめて処理を終了する（未対応）
		return;
	}
	while (is_word_or_quoted_token(*token))
	{
		append_word(word_list, *token);
		advance(token);
	}
}

static int	parse_io_number(t_token **token)
{
	int		fd;
	char	*endptr;

	fd = 0;
	if (peek(token, TK_IO_NUM))
	{
		errno = 0;
		fd = (int)strtol((*token)->word, &endptr, 10); // あとで ft_strtol に変更
		if (*endptr != '\0' || errno != 0)
			fd = -1;
		consume(token, TK_IO_NUM);
	}
	else if (peek(token, TK_REDIR_IN) || peek(token, TK_HEREDOC))
		fd = 0;
	else if (peek(token, TK_REDIR_OUT) || peek(token, TK_APPEND))
		fd = 1;
	return (fd);
}

static void	append_redir(t_redir **redir_list, t_redir *new_redir)
{
	t_redir	*current;

	if (!*redir_list)
	{
		*redir_list = new_redir;
		return ;
	}
	current = *redir_list;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new_redir;
}

static void	parse_redir(t_redir **redir_list, t_token **token)
{
	t_redir	*new_redir;
	int		fd;

	while (peek(token, TK_IO_NUM) || is_redir_token(*token))
	{
		fd = parse_io_number(token);
		if (!is_redir_token(*token))
		{
			report_error("parse_redir", NULL, "syntax error(0)");
		}
		new_redir = init_redir((*token)->type, fd);
		advance(token);     // redirect tokenをskipしている
		skip_blanks(token); // redirect token直後の空白をskipしている
		process_redir_words(&new_redir->word_list, token);
		append_redir(redir_list, new_redir);
	}
}

static t_cmd	*parse_exec(t_token **token)
{
	t_execcmd	*exec_cmd;

	exec_cmd = (t_execcmd *)init_execcmd();
	if (!exec_cmd)
	{
		report_error("parse_exec", NULL, "memory allocation failed(0)");
		return (NULL);
	}
	parse_redir(&exec_cmd->redir_list, token);
	while (!peek(token, TK_PIPE) && !peek(token, TK_EOF))
	{
		if (!is_word_or_quoted_token(*token))
		{
			report_error("parse_exec", NULL, "syntax error(2)");
		}
		append_word(&exec_cmd->word_list, *token);
		advance(token);
		parse_redir(&exec_cmd->redir_list, token);
	}
	return ((t_cmd *)exec_cmd);
}

static t_cmd	*parse_pipe(t_token **token)
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
		report_error("parser", NULL, "remaining tokens when EOF is expected");
		free_cmd(cmd);
		return (NULL);
	}
	return (cmd);
}
