/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 20:37:40 by sakitaha          #+#    #+#             */
/*   Updated: 2024/04/02 00:59:26 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp.h"

struct s_cmd	*init_execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		perror_exit("Error: malloc failed\n");
	}
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

struct s_cmd	*init_redircmd(t_cmd *subcmd, char *filepath, int oflag, int fd)
{
	struct s_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		perror_exit("Error: malloc failed\n");
	}
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->filepath = filepath;
	cmd->oflag = oflag;
	cmd->fd = fd;
	cmd->here_doc = false; // とりあえずfalseで初期化。heredocのやり方は未検討
	return ((struct cmd *)cmd);
}

struct s_cmd	*init_pipecmd(t_cmd *left, t_cmd *right)
{
	struct s_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
	{
		perror_exit("Error: malloc failed\n");
	}
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}
