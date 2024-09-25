/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_word_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:51:25 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/25 01:08:02 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"

static bool	is_continuous_token(t_arg *arg, t_arg *next_arg)
{
	if (!arg || !arg->token || !next_arg || !next_arg->token)
		return (false);
	return (arg->token->next == next_arg->token);
}

static t_arg	*append_assignment_value(t_arg *arg, char *str)
{
	t_arg	*new_arg;

	new_arg = new_arg_node(TK_DQUOTE, str);
	if (!new_arg)
	{
		return (NULL);
	}
	arg->next = new_arg;
	arg->token->next = new_arg->token;
	return (new_arg);
}

static void	divide_assignment_arg(t_arg *arg, char *eq_pos)
{
	t_arg	*next_arg;
	t_token	*next_token;
	t_arg	*after_eq_arg;
	char	*until_eq_str;

	if (!arg || !arg->token || *(eq_pos + 1) == '\0')
		return ;
	next_arg = arg->next;
	next_token = arg->token->next;
	after_eq_arg = append_assignment_value(arg, eq_pos + 1);
	if (!after_eq_arg)
		return ;
	eq_pos[1] = '\0';
	until_eq_str = ft_strdup(arg->token->word);
	free(arg->token->word);
	arg->token->word = until_eq_str;
	after_eq_arg->next = next_arg;
	after_eq_arg->token->next = next_token;
}

static bool	process_assignment_arg(t_arg *arg, bool after_equal_sign)
{
	char	*eq_pos;
	char	*dollar_pos;

	eq_pos = ft_strchr(arg->token->word, '=');
	dollar_pos = ft_strchr(arg->token->word, '$');
	if (!after_equal_sign && !eq_pos)
	{
		return (false);
	}
	if (after_equal_sign && !eq_pos && dollar_pos)
	{
		arg->token->type = TK_DQUOTE;
	}
	if (eq_pos && *(eq_pos + 1) != '\0' && dollar_pos && eq_pos < dollar_pos)
	{
		divide_assignment_arg(arg, eq_pos);
	}
	return (is_continuous_token(arg, arg->next));
}

void	expand_arg_list(t_arg *arg_list, t_mgr *mgr)
{
	t_arg	*arg;
	bool	has_export_cmd;
	bool	after_equal_sign;

	arg = arg_list;
	has_export_cmd = (ft_strcmp(arg->token->word, "export") == 0);
	after_equal_sign = false;
	while (arg)
	{
		if (arg->token->type == TK_WORD && has_export_cmd)
		{
			after_equal_sign = process_assignment_arg(arg, after_equal_sign);
		}
		if (arg->token->type == TK_WORD || arg->token->type == TK_DQUOTE)
		{
			expand_word_str(&(arg->token->word), mgr);
		}
		split_word_token(arg);
		arg = arg->next;
	}
	merge_arg_list(arg_list);
}
