/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:28:56 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/04 01:30:16 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "xlibc.h"

static void	exec_leftcmd(t_cmd *cmd, int pfd[2], t_mgr *mgr)
{
	if (xclose(pfd[0]) == -1)
	{
		exit(EXIT_FAILURE);
	}
	if (pfd[1] != STDOUT_FILENO)
	{
		if (xdup2(pfd[1], STDOUT_FILENO) == -1)
		{
			exit(EXIT_FAILURE);
		}
		if (xclose(pfd[1]) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
	exit(run_cmd(cmd, mgr));
}

static void	exec_rightcmd(t_cmd *cmd, int pfd[2], t_mgr *mgr)
{
	if (xclose(pfd[1]) == -1)
	{
		exit(EXIT_FAILURE);
	}
	if (pfd[0] != STDIN_FILENO)
	{
		if (xdup2(pfd[0], STDIN_FILENO) == -1)
		{
			exit(EXIT_FAILURE);
		}
		if (xclose(pfd[0]) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
	exit(run_cmd(cmd, mgr));
}

static pid_t	create_process(t_cmd *cmd, int pfd[2], t_mgr *mgr, bool is_left)
{
	pid_t	pid;

	pid = xfork();
	if (pid == -1)
	{
		return (-1);
	}
	if (pid == 0)
	{
		if (is_left)
			exec_leftcmd(cmd, pfd, mgr);
		else
			exec_rightcmd(cmd, pfd, mgr);
	}
	return (pid);
}

pid_t	create_left_process(t_cmd *cmd, int pfd[2], t_mgr *mgr)
{
	return (create_process(cmd, pfd, mgr, true));
}

pid_t	create_right_process(t_cmd *cmd, int pfd[2], t_mgr *mgr)
{
	return (create_process(cmd, pfd, mgr, false));
}
