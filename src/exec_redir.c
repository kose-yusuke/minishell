/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:37:56 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:37:59 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "executor.h"
#include "xlibc.h"

static bool	is_valid_redir(t_redir *redir)
{
	t_token_type	redir_type;
	int				fd;

	redir_type = redir->redir_type;
	fd = redir->fd;
	if (fd < 0 || fd > 2)
	{
		return (false);
	}
	if (redir_type == TK_REDIR_IN || redir_type == TK_HEREDOC)
	{
		return (fd == STDIN_FILENO);
	}
	else if (redir_type == TK_REDIR_OUT || redir_type == TK_APPEND)
	{
		return (fd == STDOUT_FILENO || fd == STDERR_FILENO);
	}
	return (false);
}

static int	get_open_flag(t_token_type redir_type)
{
	if (redir_type == TK_REDIR_IN || redir_type == TK_HEREDOC)
	{
		return (O_RDONLY | O_CLOEXEC);
	}
	else if (redir_type == TK_REDIR_OUT)
	{
		return (O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC);
	}
	else if (redir_type == TK_APPEND)
	{
		return (O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC);
	}
	return (-1);
}

static int	open_filepath(t_redir *redir, char *cmd_name)
{
	char	*path;
	int		oflag;
	int		fd;

	path = redir->word_list->token->word;
	oflag = get_open_flag(redir->redir_type);
	if (!path || oflag == -1)
	{
		return (-1);
	}
	if (redir->redir_type == TK_REDIR_IN || redir->redir_type == TK_HEREDOC)
	{
		fd = open(path, oflag);
	}
	else
	{
		fd = open(path, oflag, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	if (fd == -1)
	{
		sys_error(cmd_name, path);
	}
	return (fd);
}

t_status	exec_redir(t_redir *redir_list, char **argv)
{
	t_redir	*redir;
	int		filefd;
	char	*cmd_name;

	cmd_name = NULL;
	if (argv && argv[0])
		cmd_name = argv[0];
	redir = redir_list;
	while (redir)
	{
		filefd = open_filepath(redir, cmd_name);
		if (filefd == -1)
		{
			return (SC_FAILURE);
		}
		if (is_valid_redir(redir))
		{
			xdup2(filefd, redir->fd);
		}
		close(filefd);
		redir = redir->next;
	}
	return (SC_SUCCESS);
}
