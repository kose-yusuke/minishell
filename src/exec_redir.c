/* exec_redir.c */
#include "error.h"
#include "executor.h"
#include "xlibc.h"
#include <errno.h>

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
		report_error(cmd_name, path, strerror(errno));
	}
	return (fd);
}

int	exec_redir(t_execcmd *ecmd)
{
	t_redir	*redir;
	char	*cmd_name;
	int		filefd;

	redir = ecmd->redir_list;
	cmd_name = ecmd->word_list->token->word;
	while (redir)
	{
		filefd = open_filepath(redir, cmd_name);
		if (filefd == -1)
		{
			return (-1);
		}
		if (is_valid_redir(redir))
		{
			xdup2(filefd, redir->fd);
		}
		close(filefd);
		redir = redir->next;
	}
	return (0);
}
