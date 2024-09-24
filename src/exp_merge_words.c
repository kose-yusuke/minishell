/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_merge_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 23:01:28 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/25 01:03:49 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"
#include "xlibc.h"

static bool	should_merge(t_arg *arg)
{
	if (!arg || !arg->next)
		return (false);
	if (!is_word_or_quoted_token(arg->token)
		|| !is_word_or_quoted_token(arg->next->token))
		return (false);
	return (arg->token->next == arg->next->token);
}

static void	merge_adjacent_args(t_arg *arg)
{
	t_arg	*next_arg;
	char	*merged_word;
	size_t	len;

	next_arg = arg->next;
	len = ft_strlen(arg->token->word) + ft_strlen(next_arg->token->word);
	merged_word = xmalloc(len + 1);
	ft_strcpy(merged_word, arg->token->word);
	ft_strlcat(merged_word, next_arg->token->word, len + 1);
	arg->token = next_arg->token;
	free(arg->token->word);
	arg->token->word = merged_word;
	arg->next = next_arg->next;
	free(next_arg);
}

void	merge_arg_list(t_arg *arg_list)
{
	t_arg	*arg_to_check;

	arg_to_check = arg_list;
	while (arg_to_check && arg_to_check->next)
	{
		if (should_merge(arg_to_check))
			merge_adjacent_args(arg_to_check);
		else
			arg_to_check = arg_to_check->next;
	}
}
