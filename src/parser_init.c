/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:50:25 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 22:52:33 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"
#include "parser.h"
#include "utils.h"
#include "xlibc.h"

t_arg	*init_arg(t_token *token)
{
	t_arg	*new_arg;

	new_arg = xmalloc(sizeof(*new_arg));
	ft_bzero(new_arg, sizeof(*new_arg));
	new_arg->token = token;
	new_arg->next = NULL;
	return (new_arg);
}

t_redir	*init_redir(t_token_type type, int fd)
{
	t_redir	*new_redir;

	new_redir = xmalloc(sizeof(*new_redir));
	ft_bzero(new_redir, sizeof(*new_redir));
	new_redir->redir_type = type;
	new_redir->fd = fd;
	new_redir->arg_list = NULL;
	new_redir->next = NULL;
	return (new_redir);
}

t_cmd	*init_execcmd(void)
{
	t_execcmd	*cmd;

	cmd = xmalloc(sizeof(*cmd));
	ft_bzero(cmd, sizeof(*cmd));
	cmd->type = EXEC;
	cmd->arg_list = NULL;
	cmd->redir_list = NULL;
	return ((t_cmd *)cmd);
}

t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	if (!left || !right)
	{
		free_cmd(left);
		free_cmd(right);
		return (NULL);
	}
	cmd = xmalloc(sizeof(*cmd));
	ft_bzero(cmd, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}
