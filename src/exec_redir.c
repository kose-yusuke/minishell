#include "redirect.h"

static bool	is_valid_redir(t_redir *redir)
{
	if (redir->fd < 0 || redir->fd > 2)
	{
		return (false);
	}
	if (redir->fd == STDIN_FILENO && (redir->redir_type == TK_REDIR_IN
			|| redir->redir_type == TK_HEREDOC))
	{
		return (true);
	}
	if (redir->fd == STDOUT_FILENO && (redir->redir_type == TK_REDIR_OUT
			|| redir->redir_type == TK_APPEND))
	{
		return (true);
	}
	if (redir->fd == STDERR_FILENO && (redir->redir_type == TK_REDIR_OUT
			|| redir->redir_type == TK_APPEND))
	{
		return (true);
	}
	return (false);
}

static int	get_open_flag(t_token_type redir_type)
{
	if (redir_type == TK_REDIR_IN)
		return (O_RDONLY | O_CLOEXEC);
	else if (redir_type == TK_REDIR_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC);
	else if (redir_type == TK_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC);
	else if (redir_type == TK_HEREDOC)
	{
		// このケースは到達しないはず
		assert_error("Error: HEREDOC handling should not reach get_open_flag\n",
			"get_open_flag failed");
		return (-2);
	}
	assert_error("Unsupported redirection type", "get_open_flag failed");
	return (-1);
}

static int	open_filepath(t_redir *redir)
{
	int		fd;
	char	*path;
	int		oflag;
	mode_t	mode;

	path = redir->word_list->token->word;
	oflag = get_open_flag(redir->redir_type);
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (!path || oflag == -1)
	{
		assert_error("Error: get_open_flag failed\n", "open_file\n");
		return (-1);
	}
	if (redir->redir_type == TK_REDIR_IN)
		fd = open(path, oflag);
	else
		fd = open(path, oflag, mode);
	if (fd == -1) // bash: filename: Permission denied <- error msg
		assert_error("Error: open failed\n", "exec_redir failed\n");
	return (fd);
}



void	exec_redir(t_redir *redir_list, t_mgr *mgr)
{
	t_redir	*redir;
	int		filefd;

	redir = redir_list;
	while (redir)
	{
		if (redir->redir_type == TK_HEREDOC) // eofを引数に入れる
			filefd = ft_heredoc(redir->word_list->token->word);
		else
			filefd = open_filepath(redir);
		if (filefd == -1)
			return ; // error msgは先の関数内で出力, redir中止
		if (is_valid_redir(redir))
		{
			redir->backup_fd = safe_dup(redir->fd);
			safe_dup2(filefd, redir->fd);
		}
		close(filefd);
		redir = redir->next;
	}
}


// void	restore_redir(t_redir *redir)
// {
// 	if(!redir)
// 		return ;
// 	restore_redir(redir->next);

// 	if(redir->backup_fd != -1)
// 	{
// 		safe_dup2(redir->backup_fd, redir->fd);
// 		close(redir->backup_fd);
// 	}
// }
