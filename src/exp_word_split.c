/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_word_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 00:41:54 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 22:58:23 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"
#include "free.h"
#include "parser.h"
#include "xlibc.h"

t_arg	*new_arg_node(t_token_type type, char *str)
{
	t_token	*token;
	char	*start;
	char	*end;

	start = str;
	end = str + ft_strlen(str);
	token = new_token(type, &start, &end);
	if (!token)
		return (NULL);
	return (init_arg(token));
}

static t_arg	*insert_arg(t_arg *prev_arg, char *str)
{
	t_token	*blank_token;
	t_arg	*new_arg;

	blank_token = new_token(TK_BLANK, NULL, NULL);
	if (!blank_token)
		return (NULL);
	new_arg = new_arg_node(TK_WORD, str);
	if (!new_arg)
	{
		free(blank_token);
		return (NULL);
	}
	prev_arg->next = new_arg;
	prev_arg->token->next = blank_token;
	blank_token->next = new_arg->token;
	return (new_arg);
}

static bool	set_first_arg(t_arg *arg, char *str)
{
	char	*first_str;

	first_str = ft_strdup(str);
	if (!first_str)
		return (false);
	free(arg->token->word);
	arg->token->word = first_str;
	return (true);
}

static t_arg	*set_args(t_arg *arg, char **splits)
{
	t_arg	*prev_arg;
	size_t	i;

	i = 0;
	if (!set_first_arg(arg, splits[i]))
		return (NULL);
	i++;
	prev_arg = arg;
	while (splits[i])
	{
		prev_arg->next = insert_arg(prev_arg, splits[i]);
		if (!prev_arg->next)
			return (NULL);
		prev_arg = prev_arg->next;
		i++;
	}
	return (prev_arg);
}

void	split_word_token(t_arg *arg)
{
	t_arg	*next_arg;
	t_token	*next_token;
	t_arg	*last_arg;
	char	**splits;

	if (!arg || !arg->token || !has_delimiter(arg->token, IFS))
		return ;
	next_arg = arg->next;
	next_token = arg->token->next;
	splits = ft_split(arg->token->word, ' ');
	if (!splits)
		return ;
	last_arg = set_args(arg, splits);
	free_argv(splits);
	if (!last_arg)
		return ;
	last_arg->next = next_arg;
	last_arg->token->next = next_token;
}
