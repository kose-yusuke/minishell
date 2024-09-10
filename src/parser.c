/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:52:37 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 00:59:39 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "free.h"
#include "parser.h"
#include <errno.h>

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

	if (peek(token, TK_PIPE))
	{
		parser_error(*token);
		return (NULL);
	}
	cmd = parse_exec(token);
	if (cmd && peek(token, TK_PIPE))
	{
		consume(token, TK_PIPE);
		if (peek(token, TK_EOF))
		{
			parser_error(*token);
			free_cmd(cmd);
			return (NULL);
		}
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
