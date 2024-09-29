/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_word_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:51:25 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/29 05:48:02 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"

// TODO: remove this function
void	print_arg_list(char *msg, t_arg *arg_list)
{
	t_arg	*arg;
	size_t	i;

	arg = arg_list;
	i = 0;
	printf("--------------------\n");
	printf("%s\n", msg);
	while (arg)
	{
		printf("arg[%zu]: %s\n", i, arg->token->word);
		arg = arg->next;
		i++;
	}
	printf("--------------------\n");
}

static bool	is_continuous_token(t_arg *arg, t_arg *next_arg)
{
	if (!arg || !arg->token || !next_arg || !next_arg->token)
		return (false);
	return (arg->token->next == next_arg->token);
}

// 前提確認: これまでのargでdlが出現しておらず、eqが出現した場合の右辺のみword splitを行う
// KEY=VALUE の場合、KEYの部分に環境変数が含まれない場合のみ、VALUEの部分のword splitを行わない
static bool	manage_split_flags(t_arg *arg, bool *ws_off, bool *has_dlr)
{
	char	*dl_pos;
	char	*eq_pos;

	// すでにwsが禁止されている（true）または、すでにdlが出現している場合はチェック不要
	if (*ws_off || *has_dlr)
		return (is_continuous_token(arg, arg->next));
	dl_pos = ft_strchr(arg->token->word, '$');
	eq_pos = ft_strchr(arg->token->word, '=');
	// `$`と`=`の位置に応じて、フラグを設定
	if (dl_pos)
		*has_dlr = true;
	if (eq_pos && (!dl_pos || dl_pos > eq_pos))
		*ws_off = true;
	return (is_continuous_token(arg, arg->next));
}

static void	process_arg(t_arg *arg, t_mgr *mgr, bool *ws_off, bool *has_dlr)
{
	bool	is_continuous;

	is_continuous = manage_split_flags(arg, ws_off, has_dlr);
	if (arg->token->type == TK_WORD || arg->token->type == TK_DQUOTE)
		expand_word_str(&(arg->token->word), mgr);
	if (arg->token->type == TK_WORD && !*ws_off)
		split_word_token(arg);
	if (!is_continuous)
	{
		*ws_off = false;
		*has_dlr = false;
	}
}

void	expand_arg_list(t_arg *arg_list, t_mgr *mgr)
{
	t_arg	*arg;
	t_arg	*next_arg;
	bool	ws_off;
	bool	has_dlr;

	arg = arg_list;
	ws_off = false;
	has_dlr = false;
	while (arg)
	{
		next_arg = arg->next;
		process_arg(arg, mgr, &ws_off, &has_dlr);
		arg = next_arg;
	}
	merge_arg_list(arg_list);
}
