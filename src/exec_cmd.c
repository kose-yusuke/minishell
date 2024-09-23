/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 22:00:45 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/22 21:00:03 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "error.h"
#include "executor.h"
#include "free.h"
#include "signals.h"
#include "xlibc.h"
#include <sys/stat.h>
#include <sys/types.h>

static char	*search_path(const char *word, t_hash_table *hash_table)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;

	value = get_path_value(hash_table);
	while (value && *value)
	{
		ft_bzero(path, PATH_MAX);
		end = ft_strchr(value, ':');
		if (end)
			ft_strlcpy(path, value, end - value + 1);
		else
			ft_strlcpy(path, value, PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, word, PATH_MAX);
		if (access(path, X_OK) == 0)
		{
			return (ft_strdup(path));
		}
		if (end == NULL)
			return (NULL);
		value = end + 1;
	}
	return (NULL);
}

static t_status	validate_cmd_path(char **argv, char **path)
{
	struct stat	statbuf;

	if (!*path)
	{
		report_error("minishell", argv[0], "command not found");
		return (SC_NOTFOUND);
	}
	if (stat(*path, &statbuf) == -1)
	{
		sys_error("minishell", argv[0]);
		free(*path);
		return (SC_FAILURE);
	}
	if (S_ISDIR(statbuf.st_mode))
	{
		report_error("minishell", argv[0], "is a directory");
		free(*path);
		return (SC_NOEXEC);
	}
	return (SC_SUCCESS);
}

static void	handle_child_process(char **argv, char *path)
{
	extern char	**environ;

	restore_signals();
	if (execve(path, argv, environ) < 0)
	{
		sys_error("minishell", argv[0]);
		exit(SC_FAILURE);
	}
}

static t_status	handle_parent_process(pid_t pid)
{
	int	child_exit_status;

	if (waitpid(pid, &child_exit_status, 0) == -1)
	{
		sys_error(NULL, "waitpid");
		return (SC_FAILURE);
	}
	if (WIFSIGNALED(child_exit_status))
	{
		if (WTERMSIG(child_exit_status) == SIGQUIT)
		{
			write(1, "Quit: 3\n", 8);
		}
		if (WTERMSIG(child_exit_status) == SIGINT)
		{
			write(1, "\n", 1);
		}
		return (WTERMSIG(child_exit_status) + 128);
	}
	if (WIFEXITED(child_exit_status))
		return (WEXITSTATUS(child_exit_status));
	return (SC_FAILURE);
}

t_status	exec_cmd(char **argv, t_mgr *mgr)
{
	char		*path;
	t_status	status;
	pid_t		pid;

	if (is_builtin(argv[0]))
		return (exec_builtin(argv, mgr));
	if (ft_strchr(argv[0], '/') != NULL)
		path = ft_strdup(argv[0]);
	else
		path = search_path(argv[0], mgr->env_table);
	status = validate_cmd_path(argv, &path);
	if (status != SC_SUCCESS)
		return (status);
	pid = xfork();
	if (pid == -1)
	{
		free(path);
		return (SC_FAILURE);
	}
	if (pid == 0)
		handle_child_process(argv, path);
	free(path);
	return (handle_parent_process(pid));
}
