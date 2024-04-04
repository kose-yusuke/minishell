/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:59:28 by sakitaha          #+#    #+#             */
/*   Updated: 2024/04/04 15:27:18 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp.h"

/*
TODO:
1. parse時に期待されるoflagの値
	<（入力）: O_RDONLY
	>（上書き）: O_WRONLY | O_CREAT | O_TRUNC
	>>（追記）: O_WRONLY | O_CREAT | O_APPEND

2. heredoc
入力を一時ファイルに書き込んだ後、そのファイルを標準入力にリダイレクトするなどの追加の処理が必要になる

3. ファイルディスクリプタの管理
不要になったfdのクリーンアップは、いつどこで実行されるべきか？
 */
void	exec_redir(t_cmd *cmd)
{
	t_redircmd	*rcmd;
	int			oldfd;

	if (cmd->type != REDIR)
	{
		error_exit("Error: exec_redir failed\n");
	}
	rcmd = (t_redircmd *)cmd;
	oldfd = open(rcmd->filepath, rcmd->oflag);
	if (oldfd == -1)
	{
		perror_exit("Error: open failed\n");
	}
	if (oldfd != rcmd->fd)
	{
		if (dup2(oldfd, rcmd->fd) == -1)
		{
			perror_exit("Error: dup2 failed\n");
		}
		if (close(oldfd) == -1)
		{
			perror_exit("Error: close failed\n");
		}
	}
	run_cmd(rcmd->cmd);
	close(rcmd->fd);
}
