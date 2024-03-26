/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipecmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:48:03 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/26 21:57:56 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp.h"

pid_t	fork_pid(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror_exit("Error: fork failed\n");
	}
	return (pid);
}

static void	execute_leftcmd(t_pipecmd *pcmd, int pfd[2])
{
	// 不要なRead endを閉じる
	if (close(pfd[0]) == -1)
	{
		perror_exit("Error: close failed\n");
	}
	if (pfd[1] != STDOUT_FILENO)
	{
		if (dup2(pfd[1], STDOUT_FILENO) == -1)
		{
			perror_exit("Error: dup2 failed\n");
		}
		if (close(pfd[1]) == -1)
		{
			perror_exit("Error: close failed\n");
		}
	}
	run_cmd(pcmd->left);
	error_exit("Error: run_cmd failed\n");
}

static void	execute_rightcmd(t_pipecmd *pcmd, int pfd[2])
{
	// 不要なWrite endを閉じる
	if (close(pfd[1]) == -1)
	{
		perror_exit("Error: close failed\n");
	}
	if (pfd[0] != STDIN_FILENO)
	{
		if (dup2(pfd[0], STDIN_FILENO) == -1)
		{
			perror_exit("Error: dup2 failed\n");
		}
		if (close(pfd[0]) == -1)
		{
			perror_exit("Error: close failed\n");
		}
	}
	run_cmd(pcmd->right);
	error_exit("Error: run_cmd failed\n");
}

void	execute_pipecmd(t_cmd *cmd)
{
	t_pipecmd	*pcmd;
	int			pfd[2];
	pid_t		left_pid;
	pid_t		right_pid;

	if (cmd->type != PIPE)
	{
		error_exit("Error: run_pipecmd called with non-pipe command\n");
	}
	pcmd = (t_pipecmd *)cmd;
	if (pipe(pfd) == -1)
	{
		perror_exit("Error: pipe failed\n");
	}
	left_pid = fork_pid();
	if (left_pid == 0)
	{
		execute_leftcmd(pcmd, pfd);
	}
	right_pid = fork_pid();
	if (right_pid == 0)
	{
		execute_rightcmd(pcmd, pfd);
	}
	if (close(pfd[0]) == -1 || close(pfd[1]) == -1)
	{
		perror_exit("Error: close failed\n");
	}
	if (waitpid(left_pid, NULL, 0) == -1 || waitpid(right_pid, NULL, 0) == -1)
	{
		perror_exit("Error: wait failed\n");
	}
	exit(EXIT_SUCCESS);
}
