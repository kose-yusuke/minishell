/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:48:03 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 21:46:18 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"
#include "minishell.h"

void	free_word(t_arg *arg_list)
{
	t_arg	*next;

	while (arg_list)
	{
		next = arg_list->next;
		free(arg_list);
		arg_list = next;
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free_word(redir->arg_list);
		free(redir);
		redir = next;
	}
}

static void	free_right_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_arg		*next;

	ecmd = (t_execcmd *)cmd;
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		while (ecmd->arg_list)
		{
			next = ecmd->arg_list->next;
			free(ecmd->arg_list);
			ecmd->arg_list = next;
		}
		free_redir(ecmd->redir_list);
	}
	else if (cmd->type == PIPE)
	{
		free_right_cmd(((t_pipecmd *)cmd)->left);
		free_right_cmd(((t_pipecmd *)cmd)->right);
	}
	free(cmd);
}

void	free_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		free_word(ecmd->arg_list);
		ecmd->arg_list = NULL;
		free_redir(((t_execcmd *)cmd)->redir_list);
	}
	else if (cmd->type == PIPE)
	{
		free_cmd(((t_pipecmd *)cmd)->left);
		free_right_cmd(((t_pipecmd *)cmd)->right);
	}
	free(cmd);
}
