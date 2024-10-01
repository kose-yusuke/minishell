/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_word_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:51:25 by sakitaha          #+#    #+#             */
/*   Updated: 2024/10/01 02:25:46 by sakitaha         ###   ########.fr       */
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

static bool	manage_split_flags(t_arg *arg, bool *ws_off, bool *dlr_on)
{
	char	*dl_pos;
	char	*eq_pos;

	if (*ws_off || *dlr_on)
		return (is_continuous_token(arg, arg->next));
	dl_pos = ft_strchr(arg->token->word, '$');
	eq_pos = ft_strchr(arg->token->word, '=');
	if (dl_pos)
		*dlr_on = true;
	if (eq_pos && (!dl_pos || dl_pos > eq_pos))
		*ws_off = true;
	return (is_continuous_token(arg, arg->next));
}

static void	process_arg(t_arg *arg, t_mgr *mgr, bool *ws_off, bool *dlr_on)
{
	bool	is_continuous;

	is_continuous = manage_split_flags(arg, ws_off, dlr_on);
	if (arg->token->type == TK_WORD || arg->token->type == TK_DQUOTE)
		expand_word_str(&(arg->token->word), mgr);
	if (arg->token->type == TK_WORD && !*ws_off)
		split_word_token(arg);
	if (!is_continuous)
	{
		*ws_off = false;
		*dlr_on = false;
	}
}

void	expand_arg_list(t_arg *arg_list, t_mgr *mgr)
{
	t_arg	*arg;
	t_arg	*next_arg;
	bool	ws_off;
	bool	dlr_on;

	arg = arg_list;
	ws_off = false;
	dlr_on = false;
	while (arg)
	{
		next_arg = arg->next;
		process_arg(arg, mgr, &ws_off, &dlr_on);
		arg = next_arg;
	}
	merge_arg_list(arg_list);
}
