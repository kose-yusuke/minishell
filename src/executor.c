/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 22:11:11 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 21:48:42 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "executor.h"
#include "free.h"
#include "xlibc.h"

static bool	save_std_fds(int *saved_stdin, int *saved_stdout, int *saved_stderr)
{
	*saved_stdin = xdup(STDIN_FILENO);
	if (*saved_stdin == -1)
	{
		return (false);
	}
	*saved_stdout = xdup(STDOUT_FILENO);
	if (*saved_stdout == -1)
	{
		close(*saved_stdin);
		return (false);
	}
	*saved_stderr = xdup(STDERR_FILENO);
	if (*saved_stderr == -1)
	{
		close(*saved_stdin);
		close(*saved_stdout);
		return (false);
	}
	return (true);
}

static void	restore_std_fds(int saved_stdin, int saved_stdout, int saved_stderr)
{
	xdup2(saved_stdin, STDIN_FILENO);
	xdup2(saved_stdout, STDOUT_FILENO);
	xdup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
}

static t_status	process_exec(t_execcmd *ecmd, t_mgr *mgr)
{
	int		status;
	int		saved_stdin;
	int		saved_stdout;
	int		saved_stderr;
	char	**argv;

	if (!save_std_fds(&saved_stdin, &saved_stdout, &saved_stderr))
	{
		return (SC_FAILURE);
	}
	argv = convert_list_to_array(ecmd->arg_list);
	status = exec_redir(ecmd->redir_list, argv);
	if (status == SC_SUCCESS && argv)
		status = exec_cmd(argv, mgr);
	free_argv(argv);
	restore_std_fds(saved_stdin, saved_stdout, saved_stderr);
	mgr->exit_status = status;
	return (status);
}

t_status	run_cmd(t_cmd *cmd, t_mgr *mgr)
{
	if (!cmd || !mgr)
	{
		return (SC_FAILURE);
	}
	if (g_status != 0 && mgr->exit_status == 0)
	{
		mgr->exit_status = g_status;
		return (g_status);
	}
	if (cmd->type == EXEC)
	{
		return (process_exec((t_execcmd *)cmd, mgr));
	}
	else if (cmd->type == PIPE)
	{
		return (exec_pipe((t_pipecmd *)cmd, mgr));
	}
	return (SC_FAILURE);
}
