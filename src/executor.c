/* executor.c - コマンドの実行とプロセス管理に関する関数の実装。 */
#include "executor.h"

static pid_t	fork_pid(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		assert_error("Error: fork failed\n", "fork_pid failed\n");
	}
	return (pid);
}

static void	exec_leftcmd(t_pipecmd *pcmd, int pfd[2], t_mgr *mgr)
{
	// 不要なRead endを閉じる
	if (close(pfd[0]) == -1)
	{
		assert_error("Error: close failed\n", "exec_leftcmd failed\n");
	}
	if (pfd[1] != STDOUT_FILENO)
	{
		if (dup2(pfd[1], STDOUT_FILENO) == -1)
		{
			assert_error("Error: dup2 failed\n", "exec_leftcmd failed\n");
		}
		if (close(pfd[1]) == -1)
		{
			assert_error("Error: close failed\n", "exec_leftcmd failed\n");
		}
	}
	run_cmd(pcmd->left, mgr);
	assert_error("Error: run_cmd failed\n", "exec_leftcmd failed\n");
}

static void	exec_rightcmd(t_pipecmd *pcmd, int pfd[2])
{
	// 不要なWrite endを閉じる
	if (close(pfd[1]) == -1)
	{
		assert_error("Error: close failed\n", "exec_rightcmd failed\n");
	}
	if (pfd[0] != STDIN_FILENO)
	{
		if (dup2(pfd[0], STDIN_FILENO) == -1)
		{
			assert_error("Error: dup2 failed\n", "exec_rightcmd failed\n");
		}
		if (close(pfd[0]) == -1)
		{
			assert_error("Error: close failed\n", "exec_rightcmd failed\n");
		}
	}
	run_cmd(pcmd->right, mgr);
	assert_error("Error: run_cmd failed\n", "exec_rightcmd failed\n");
}

static void	exec_pipe(t_cmd *cmd, t_mgr *mgr)
{
	t_pipecmd	*pcmd;
	int			pfd[2];
	pid_t		left_pid;
	pid_t		right_pid;

	if (cmd->type != PIPE)
	{
		assert_error("Error: unexpected cmd", "exec_pipe failed\n");
	}
	pcmd = (t_pipecmd *)cmd;
	if (pipe(pfd) == -1)
	{
		assert_error("Error: pipe failed\n", "exec_pipe failed\n");
	}
	left_pid = fork_pid();
	if (left_pid == 0)
	{
		exec_leftcmd(pcmd, pfd, mgr);
	}
	right_pid = fork_pid();
	if (right_pid == 0)
	{
		exec_rightcmd(pcmd, pfd, mgr);
	}
	if (close(pfd[0]) == -1 || close(pfd[1]) == -1)
	{
		assert_error("Error: close failed\n", "exec_pipe failed\n");
	}
	if (waitpid(left_pid, NULL, 0) == -1 || waitpid(right_pid, NULL, 0) == -1)
	{
		assert_error("Error: waitpid failed\n", "exec_pipe failed\n");
	}
	exit(EXIT_SUCCESS);
}

static void	exec_cmd(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	char		**argv;
	extern char	**environ;
	char		*path;

	if (cmd->type != EXEC)
	{
		assert_error("Error: unexpected cmd", "exec_cmd failed\n");
	}
	ecmd = (t_execcmd *)cmd;
	if (!ecmd->word_list || !ecmd->word_list->token)
	{
		// TODO: execveを使わない時には自力でfdをクローズする
		// close_fd();
		exit(0);
	}
	path = search_path(ecmd->word_list->token); //未実装
	// TODO: ここで word_list を argv に変換する。仮にNULL
	argv = NULL;
	// TODO: ここ、もしくはexpandの段階でpathの解決を行う (search_path関数)
	// TODO: 環境変数はまだ実装していないから第三引数は正しくない仮
	// builtinコマンドの場合は、ここで処理を行う
	if (execve(path, argv, environ) < 0)
	{
		assert_error("Error: execve failed\n", "exec_cmd failed\n");
	}
	// TODO: execveが失敗すると、open on O_CLOSEXEC が機能しない
	// そのため、自力でfdをクローズする必要がある
	assert_error("Error: execve failed\n", "exec_cmd failed\n");
	// close_fd();
}

void	exec_redir(t_cmd *cmd, t_mgr *mgr)
{
	int			oldfd;
	t_execcmd	*ecmd;
	t_redir		**current_redir;
	char		*filepath;
	int			oflag;

	if (cmd->type != EXEC)
	{
		assert_error("Error: unexpected cmd", "exec_redir failed\n");
	}
	ecmd = (t_execcmd *)cmd;
	redir_list = ecmd->redir_list;
	// typeとfdが共に重複するredirectは、先頭のものしか実行されない <- 未実装
	while (redir_list)
	{
		if (redir_list->redir_type == TK_HEREDOC)
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
				assert_error("Error: dup2 failed\n", "exec_redir failed\n");
			}
			if (close(oldfd) == -1)
			{
				assert_error("Error: close failed\n", "exec_redir failed\n");
			}
		}
		redir_list = redir_list->next;
	}
}

void	run_cmd(t_cmd *cmd, t_mgr *mgr)
{
	if (!mgr || !mgr->env_table)
	{
		assert_error("Error: ", "run_cmd failed\n");
	}
	else if (!cmd || cmd->type == NONE)
	{
		return ; // or exit(0); ?
	}
	else if (cmd->type == EXEC)
	{
		// exec_redir(cmd);
		exec_cmd(cmd, mgr);
		// reset_fd(cmd); 未実装
	}
	else if (cmd->type == PIPE)
	{
		exec_pipe(cmd, mgr);
	}
	else
	{
		assert_error("Error: ", "run_cmd failed\n");
	}
}

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
