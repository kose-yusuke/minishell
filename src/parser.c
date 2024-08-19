/* parser.c - パーサー関連の関数 */
#include "free.h"
#include "parser.h"

static void	append_word(t_word **word_list, t_word *new_word)
{
	t_word	*current;

	if (!*word_list)
	{
		*word_list = new_word;
		return ;
	}
	current = *word_list;
	while (current->next)
		current = current->next;
	current->next = new_word;
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
		current = current->next;
	current->next = new_redir;
}

static int	process_redir_words(t_word **word_list, t_token **token)
{
	if (!is_word_or_quoted_token(*token))
	{
		parser_error(*token);
		return (1);
	}
	while (is_word_or_quoted_token(*token))
	{
		append_word(word_list, init_word(*token));
		advance(token);
	}
	return (0);
}

static int	parse_io_number(t_token **token)
{
	int		fd;
	char	*endptr;

	fd = 0;
	if (peek(token, TK_IO_NUM))
	{
		errno = 0;
		// TODO: あとで ft_strtol に変更
		fd = (int)strtol((*token)->word, &endptr, 10);
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

static int	parse_redir(t_redir **redir_list, t_token **token)
{
	t_redir	*new_redir;
	int		fd;

	while (peek(token, TK_IO_NUM) || is_redir_token(*token))
	{
		fd = parse_io_number(token);
		if (!is_redir_token(*token))
		{
			parser_error(*token);
			return (1);
		}
		new_redir = init_redir((*token)->type, fd);
		advance(token);     // redirect tokenをskipしている
		skip_blanks(token); // redirect token直後の空白をskipしている
		if (process_redir_words(&new_redir->word_list, token) != 0)
		{
			free_redir(new_redir);
			return (1);
		}
		append_redir(redir_list, new_redir);
	}
	return (0);
}

static t_cmd	*parse_exec(t_token **token)
{
	t_execcmd	*exec_cmd;

	exec_cmd = (t_execcmd *)init_execcmd();
	if (parse_redir(&exec_cmd->redir_list, token) != 0)
	{
		free_cmd((t_cmd *)exec_cmd);
		return (NULL);
	}
	while (!peek(token, TK_PIPE) && !peek(token, TK_EOF))
	{
		if (!is_word_or_quoted_token(*token))
		{
			parser_error(*token);
			free_cmd((t_cmd *)exec_cmd);
			return (NULL);
		}
		append_word(&exec_cmd->word_list, init_word(*token));
		advance(token);
		if (parse_redir(&exec_cmd->redir_list, token) != 0)
		{
			free_cmd((t_cmd *)exec_cmd);
			return (NULL);
		}
	}
	return ((t_cmd *)exec_cmd);
}

static t_cmd	*parse_pipe(t_token **token)
{
	t_cmd	*cmd;

	cmd = parse_exec(token);
	if (cmd && peek(token, TK_PIPE))
	{
		consume(token, TK_PIPE);
		cmd = init_pipecmd(cmd, parse_pipe(token));
	}
	return (cmd);
}

t_cmd	*parser(t_token *token)
{
	t_cmd	*cmd;
	t_token	*token_ps;

	token_ps = token;
	cmd = parse_pipe(&token_ps);
	if (!peek(&token_ps, TK_EOF) || !cmd)
	{
		free_cmd(cmd);
		return (NULL);
	}
	return (cmd);
}
