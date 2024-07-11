#include "redirect.h"

int	get_open_flag(t_token_type redir_type)
{
	if (redir_type == TK_REDIR_IN)
		return (O_RDONLY | O_CLOEXEC);
	else if (redir_type == TK_REDIR_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC);
	else if (redir_type == TK_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC);
	else if (redir_type == TK_HEREDOC)
		return (-2);
	assert_error("Unsupported redirection type", "get_open_flag failed");
	return (-1);
}

void	handle_duplicate_fd(t_fd_mgr *fd_mgr, int fd)
{
	t_fd_mgr	*new_fd;

	new_fd = (t_fd_mgr *)malloc(sizeof(t_fd_mgr));
	if (!new_fd)
	{
		assert_error("Error: malloc failed\n", "handle_duplicate_fd failed\n");
	}
	new_fd->fd = fd;
	new_fd->next = fd_mgr->next;
	fd_mgr->next = new_fd;
}

int is_fd_valid(int fd) {
    return write(fd, "", 0) != -1 || errno != EBADF;
}

void	process_redir_out(t_redir *redir, t_mgr *mgr)
{
	char	*filepath;
	int		oflag;
	int		fd;

	if (redir->opened)
	{
		return ;
	}
	filepath = redir->word_list->token->word; // TODO: expandで解決？確認
	oflag = get_open_flag(redir->redir_type);
	if (!filepath || oflag == -1)
	{
		assert_error("Error: get_open_flag failed\n", "process_redir_in\n");
	}
	fd = open(filepath, oflag, 0644);
	if (fd == -1)
	{
		assert_error("Error: open failed\n", "process_redir_in");
	}
	if (fd != redir->fd)
	{
		if (dup2(fd, redir->fd) == -1)
		{
			assert_error("Error: dup2 failed\n", "process_redir_in\n");
		}
		if (close(fd) == -1)
		{
			assert_error("Error: close failed\n", "process_redir_in\n");
		}
	}
}

void	process_redir_in(t_execcmd *ecmd, t_mgr *mgr)
{
	t_redir	*redir;
	char	*filepath;

	redir = ecmd->redir_list;
	while (redir)
	{
		if ((redir->redir_type != TK_REDIR_IN && redir->redir_type != TK_APPEND)
			|| redir->opened)
		{
			redir = redir->next;
			continue ;
		}
		filepath = redir->word_list->token->word; // TODO: 確認
	}
}

/* 一個分のredirectを実行する */
void	exec_redir(t_redir *redir, t_mgr *mgr)
{
	if (redir->redir_type == TK_REDIR_OUT || redir->redir_type == TK_APPEND)
	{
		process_redir_out(redir, mgr);
	}
	else if (redir->redir_type == TK_REDIR_IN
		|| redir->redir_type == TK_HEREDOC)
	{
		process_redir_in(redir, mgr);
	}
}

void	exec_redir_list(t_cmd *cmd, t_mgr *mgr)
{
	int			oldfd;
	t_execcmd	*ecmd;
	t_redir		**current_redir;
	char		*filepath;
	int			oflag;

	ecmd = (t_execcmd *)cmd;
	*current_redir = ecmd->redir_list;
	while (*current_redir)
	{
		if (*current_redir->redir_type == TK_HEREDOC)
		{
			// TODO: heredocの処理
			redir_list = redir_list->next;
			continue ;
		}
		filepath = ecmd->word_list->token->word;    // expandで解決
		oflag = get_o_flag(redir_list->redir_type); // 未実装
		oldfd = open(filepath, oflag);
		if (oldfd == -1)
		{
			assert_error("Error: open failed\n", "exec_redir failed\n");
		}
		if (oldfd != redir_list->fd)
		{
			if (dup2(oldfd, redir_list->fd) == -1)
			{
				assert_error("Error: dup2 failed\n", "exec_redir failed\n"); // まちがい 標準入出力につけかえる
			}
			if (close(oldfd) == -1) // ?
			{
				assert_error("Error: close failed\n", "exec_redir failed\n");
			}
		}
		redir_list = redir_list->next;
	}

/*
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>

void	redirect(int targetfd, char *filename)
{
	int filefd, stashed_targetfd;
	// 1. Redirect先のfdをopenする
	filefd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	filefd = stashfd(filefd); // filefdを退避させる
	// 2. Redirectする
	stashed_targetfd = stashfd(targetfd); // targetfdを退避させる
	if (filefd != targetfd)
	{
		dup2(filefd, targetfd); // filefdをtargetfdに複製する（元々のtargetfdは閉じられる）
		close(filefd);
	}
	// 3. コマンドを実行する
	exec_command();
	// 4. Redirectしていたのを元に戻す
	dup2(stashed_targetfd, targetfd);
		// 退避していたstashed_targetfdをtargetfdに複製する（元々のtargetfd）
}

int	stashfd(int fd)
{
	int	stashfd;

	stashfd = fcntl(fd, F_DUPFD, 10);
	if (stashfd < 0)
		fatal_error("fcntl");
	if (close(fd) < 0)
		fatal_error("close");
	return (stashfd);
}
stashfdという関数は引数として受け取ったfdを、他と被らないであろう10以上のfdに退避させています。 fcntl はminishellの使用可能関数には入っていませんが、簡単なので使用しています。後ほど他の使用可能関数を用いてstashfdを書き直すことはできるので、ひとまずはこんなものでいいでしょう。

- redirectが複数行われる際は、redirectしたのと逆の順番でreset_redirectをしていく必要があることに注意が必要です。例えば `echo hello >f1 >f2 >f3`  のようなredirectを行った時、どのようにfdが増えたり減ったり変化していき、最終的にどのようにresetされるかの流れは先ほどの章に書いた通り。


 */
