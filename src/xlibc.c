/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xlibc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:45:50 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 00:46:36 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "xlibc.h"
#include <stdio.h>
#include <unistd.h>

void	*xmalloc(size_t bytes)
{
	void	*temp;

	temp = malloc(bytes);
	if (!temp)
		memory_error_and_exit("xmalloc");
	return (temp);
}

int	xclose(int fd)
{
	if (close(fd) == -1)
	{
		sys_error("minishell", "close");
		return (-1);
	}
	return (0);
}

int	xdup(int oldfd)
{
	int	fd;

	fd = dup(oldfd);
	if (fd == -1)
		sys_error("minishell", "dup");
	return (fd);
}

int	xdup2(int oldfd, int newfd)
{
	newfd = dup2(oldfd, newfd);
	if (newfd == -1)
		sys_error("minishell", "dup2");
	return (newfd);
}

pid_t	xfork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		sys_error("minishell", "fork");
	return (pid);
}
