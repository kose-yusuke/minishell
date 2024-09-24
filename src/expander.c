/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:36:48 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/25 01:03:28 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"
#include "heredoc.h"

static bool	is_quoted_heredoc(t_arg *arg_list)
{
	t_arg	*arg;

	arg = arg_list;
	while (arg)
	{
		if (arg->token->type == TK_SQUOTE || arg->token->type == TK_DQUOTE)
			return (true);
		arg = arg->next;
	}
	return (false);
}

static void	merge_heredoc_delimi(t_arg *arg_list)
{
	bool	has_quote;

	has_quote = is_quoted_heredoc(arg_list);
	merge_arg_list(arg_list);
	if (has_quote)
	{
		arg_list->token->type = TK_SQUOTE;
	}
}

static void	expand_redir_word(t_arg *arg_list, t_mgr *mgr)
{
	t_arg	*arg;
	t_arg	*next_arg;

	arg = arg_list;
	while (arg)
	{
		next_arg = arg->next;
		if (arg->token->type == TK_WORD || arg->token->type == TK_DQUOTE)
		{
			expand_word_str(&(arg->token->word), mgr);
		}
		split_word_token(arg);
		arg = next_arg;
	}
	merge_arg_list(arg_list);
}

static void	expand_redir_list(t_redir *redir_list, t_mgr *mgr)
{
	t_redir	*redir;

	redir = redir_list;
	while (redir && g_status == 0)
	{
		if (redir->redir_type == TK_HEREDOC)
		{
			merge_heredoc_delimi(redir_list->arg_list);
			ft_heredoc(redir, mgr);
		}
		else
		{
			expand_redir_word(redir_list->arg_list, mgr);
		}
		redir = redir->next;
	}
}

void	run_expansion(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;

	if (!cmd || !mgr || g_status != 0)
		return ;
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		expand_redir_list(ecmd->redir_list, mgr);
		expand_arg_list(ecmd->arg_list, mgr);
		mgr->exit_status = SC_SUCCESS;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		run_expansion(pcmd->left, mgr);
		run_expansion(pcmd->right, mgr);
	}
}
