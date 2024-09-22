/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:52:50 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:20:26 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "free.h"
#include "lexer.h"
#include "xlibc.h"

static t_token_type	get_token_type(char **ps, char **q, char **eq)
{
	t_token_type	type;

	type = get_blank_type(ps);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	type = get_op_type(ps);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	type = get_quoted_type(ps, q, eq);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	return (get_word_or_ionum_type(ps, q, eq));
}

t_token	*new_token(t_token_type type, char **q, char **eq)
{
	t_token	*new_token;
	size_t	len;

	if (type == TK_UNDEF_TOKEN || type == TK_PARSE_ERROR)
		return (NULL);
	new_token = xmalloc(sizeof(t_token));
	ft_bzero(new_token, sizeof(t_token));
	new_token->type = type;
	if (is_word_or_quoted_token(new_token) || is_io_num_token(new_token))
	{
		if (!q || !eq || !*q)
		{
			free(new_token);
			return (NULL);
		}
		len = *eq - *q;
		new_token->word = ft_strndup(*q, len);
	}
	return (new_token);
}

static t_token	*add_eof_token(t_token *cur_token, t_token *head_token)
{
	cur_token->next = new_token(TK_EOF, NULL, NULL);
	if (!cur_token->next)
	{
		free_tokens(head_token);
		return (NULL);
	}
	return (head_token->next);
}

static t_token	*create_tokens(char *s, t_token *head_token)
{
	t_token	*cur_token;
	char	*q;
	char	*eq;

	cur_token = head_token;
	while (*s)
	{
		q = NULL;
		eq = NULL;
		cur_token->next = new_token(get_token_type(&s, &q, &eq), &q, &eq);
		if (!cur_token->next)
		{
			free_tokens(head_token->next);
			return (NULL);
		}
		cur_token = cur_token->next;
	}
	return (cur_token);
}

t_token	*lexer(char *s)
{
	t_token	head_token;
	t_token	*cur_token;

	head_token.next = NULL;
	cur_token = create_tokens(s, &head_token);
	if (!cur_token)
		return (NULL);
	return (add_eof_token(cur_token, head_token.next));
}
