/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:56:17 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 00:59:16 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "free.h"
#include "ft_strtol.h"
#include "parser.h"

void	append_word(t_word **word_list, t_word *new_word)
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
		if (process_redir_words(&new_redir->word_list, token) != 0)
		{
			free_redir(new_redir);
			return (1);
		}
		append_redir(redir_list, new_redir);
	}
	return (0);
}
