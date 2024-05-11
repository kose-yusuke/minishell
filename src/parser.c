/* parser.c - パーサー関連の関数 */
#include "parser.h"

/*
全体としてclean upは後回しにしているので後で実装する
関数の中ではなく、呼び出し側でerror handlingするようにする
*/
t_cmd	*init_execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = calloc(1, sizeof(*cmd));
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

	cmd = calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		perror_exit("Error: memory allocation failed\n");
	}
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_redir	*init_redir(t_token_type type, int fd, t_redir *next)
{
	struct s_redir	*new_redir;

	new_redir = calloc(1, sizeof(*new_redir));
	if (!new_redir)
	{
		perror_exit("Error: memory allocation failed\n");
	}
	new_redir->redir_type = type;
	new_redir->fd = fd;
	new_redir->word_list = NULL;
	new_redir->next = next;
	return (new_redir);
}

void	append_word(t_word **word_list, t_token *token)
{
	t_word	*new_word;
	t_word	*current;

	new_word = calloc(1, sizeof(*new_word));
	if (!new_word)
	{
		// TODO: clean up for allocated memory
		perror_exit("Error: malloc failed\n");
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

void	process_redir_words(t_word **word_list, t_token **token)
{
	while (is_word_or_quoted_token(*token))
	{
		append_word(word_list, *token);
		advance(token);
	}
}

// TODO: fdのエラー対応については、expansionの段階で対応する
int	parse_io_number(t_token **token)
{
	int		fd;
	char	*endptr;

	fd = 0;
	if (peek(token, TK_IO_NUM))
	{
		errno = 0;
		fd = strtol((*token)->word, &endptr, 10);
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

void	prepend_redir(t_redir **redir_list, t_token **token)
{
	t_redir	*new_redir;
	int		fd;

	while (peek(token, TK_IO_NUM) || is_redir_token(*token))
	{
		fd = parse_io_number(token);
		if (!is_redir_token(*token))
		{
			// TODO: clean up for allocated memory
			error_exit("syntax error\n");
		}
		new_redir = init_redir((*token)->type, fd, *redir_list);
		advance(token);
		skip_blanks(token);
		if (!is_word_or_quoted_token(*token))
		{
			// TODO: clean up for allocated memory
			error_exit("syntax error\n");
		}
		process_redir_words(&new_redir->word_list, token);
		*redir_list = new_redir;
	}
}

t_cmd	*parse_exec(t_token **token)
{
	t_execcmd	*exec_cmd;

	exec_cmd = (t_execcmd *)init_execcmd();
	if (!exec_cmd)
	{
		// TODO: clean up for allocated memory
		perror_exit("Error: malloc failed\n");
	}
	prepend_redir(&exec_cmd->redir_list, token);
	while (!peek(token, TK_PIPE) && !peek(token, TK_EOF))
	{
		if (!is_word_or_quoted_token(*token))
		{
			// TODO: clean up for allocated memory
			error_exit("syntax error\n");
		}
		append_word(&exec_cmd->word_list, *token);
		advance(token);
		prepend_redir(&exec_cmd->redir_list, token);
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
		// TODO: clean up for allocated memory
		error_exit("syntax error\n");
	}
	return (cmd);
}
