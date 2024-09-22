/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:48:03 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 00:50:59 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"
#include "minishell.h"

void	free_word(t_word *word)
{
	t_word	*next;

	while (word)
	{
		next = word->next;
		free(word);
		word = next;
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free_word(redir->word_list);
		free(redir);
		redir = next;
	}
}

static void	free_right_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_word		*next;

	ecmd = (t_execcmd *)cmd;
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		while (ecmd->word_list)
		{
			next = ecmd->word_list->next;
			free(ecmd->word_list);
			ecmd->word_list = next;
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
		free_word(ecmd->word_list);
		ecmd->word_list = NULL;
		free_redir(((t_execcmd *)cmd)->redir_list);
	}
	else if (cmd->type == PIPE)
	{
		free_cmd(((t_pipecmd *)cmd)->left);
		free_right_cmd(((t_pipecmd *)cmd)->right);
	}
	free(cmd);
}
