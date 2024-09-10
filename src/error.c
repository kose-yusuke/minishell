/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:11:11 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 01:15:45 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "minishell.h"
#include "utils.h"
#include <errno.h>
#include <unistd.h>

void	memory_error_and_exit(char *func_name)
{
	write(2, func_name, ft_strlen(func_name));
	write(2, ": out of virtual memory\n", 24);
	exit(SC_FATAL_ERROR);
}

void	assert_error(const char *msg, char *location)
{
	write(STDERR_FILENO, "Assert Error: ", 14);
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	if (location)
	{
		write(STDERR_FILENO, " near ", 6);
		write(STDERR_FILENO, location, ft_strlen(location));
	}
	write(STDERR_FILENO, "\n", 1);
	exit(255);
}

void	error_exit(const char *msg, int exit_status)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(exit_status);
}

void	report_error(char *cmd, char *file, char *info)
{
	if (cmd && *cmd)
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
	else
		write(STDERR_FILENO, "minishell", 9);
	write(STDERR_FILENO, ":", 1);
	if (file && *file)
	{
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, file, ft_strlen(file));
		write(STDERR_FILENO, ":", 1);
	}
	if (info)
	{
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, info, ft_strlen(info));
	}
	write(STDERR_FILENO, "\n", 1);
}

void	sys_error(char *cmd, char *msg)
{
	int	e;

	e = errno;
	if (cmd && *cmd)
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
	else
		write(STDERR_FILENO, "minishell", 9);
	write(STDERR_FILENO, ": ", 2);
	if (msg && *msg)
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
	}
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, strerror(e), ft_strlen(strerror(e)));
	write(STDERR_FILENO, "\n", 1);
}
