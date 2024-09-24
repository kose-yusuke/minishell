/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:56:17 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/25 01:09:48 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "free.h"
#include "ft_strtol.h"
#include "parser.h"

void	append_arg(t_arg **arg_list, t_arg *new_arg)
{
	t_arg	*current;

	if (!*arg_list)
	{
		*arg_list = new_arg;
		return ;
	}
	current = *arg_list;
	while (current->next)
		current = current->next;
	current->next = new_arg;
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

static int	process_redir_args(t_arg **arg_list, t_token **token)
{
	if (!is_word_or_quoted_token(*token))
	{
		parser_error(*token);
		return (1);
	}
	while (is_word_or_quoted_token(*token))
	{
		append_arg(arg_list, init_arg(*token));
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
		fd = (int)ft_strtol((*token)->word, &endptr, 10);
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

int	parse_redir(t_redir **redir_list, t_token **token)
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
		advance(token);
		skip_blanks(token);
		if (process_redir_args(&new_redir->arg_list, token) != 0)
		{
			free_redir(new_redir);
			return (1);
		}
		append_redir(redir_list, new_redir);
	}
	return (0);
}
