/* executor.c - コマンドの実行とプロセス管理に関する関数の実装。 */
#include "executor.h"

static pid_t	fork_pid(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror_exit("Error: fork failed\n");
	}
	return (pid);
}

static void	exec_leftcmd(t_pipecmd *pcmd, int pfd[2])
{
	// 不要なRead endを閉じる
	if (close(pfd[0]) == -1)
	{
		perror_exit("Error: close failed\n");
	}
	if (pfd[1] != STDOUT_FILENO)
	{
		if (dup2(pfd[1], STDOUT_FILENO) == -1)
		{
			perror_exit("Error: dup2 failed\n");
		}
		if (close(pfd[1]) == -1)
		{
			perror_exit("Error: close failed\n");
		}
	}
	run_cmd(pcmd->left);
	error_exit("Error: run_cmd failed\n");
}

static void	exec_rightcmd(t_pipecmd *pcmd, int pfd[2])
{
	// 不要なWrite endを閉じる
	if (close(pfd[1]) == -1)
	{
		perror_exit("Error: close failed\n");
	}
	if (pfd[0] != STDIN_FILENO)
	{
		if (dup2(pfd[0], STDIN_FILENO) == -1)
		{
			perror_exit("Error: dup2 failed\n");
		}
		if (close(pfd[0]) == -1)
		{
			perror_exit("Error: close failed\n");
		}
	}
	run_cmd(pcmd->right);
	error_exit("Error: run_cmd failed\n");
}

static void	exec_pipe(t_cmd *cmd)
{
	t_pipecmd	*pcmd;
	int			pfd[2];
	pid_t		left_pid;
	pid_t		right_pid;

	if (cmd->type != PIPE)
	{
		error_exit("Error: run_pipecmd called with non-pipe command\n");
	}
	pcmd = (t_pipecmd *)cmd;
	if (pipe(pfd) == -1)
	{
		perror_exit("Error: pipe failed\n");
	}
	left_pid = fork_pid();
	if (left_pid == 0)
	{
		exec_leftcmd(pcmd, pfd);
	}
	right_pid = fork_pid();
	if (right_pid == 0)
	{
		exec_rightcmd(pcmd, pfd);
	}
	if (close(pfd[0]) == -1 || close(pfd[1]) == -1)
	{
		perror_exit("Error: close failed\n");
	}
	if (waitpid(left_pid, NULL, 0) == -1 || waitpid(right_pid, NULL, 0) == -1)
	{
		perror_exit("Error: wait failed\n");
	}
	exit(EXIT_SUCCESS);
}

static void	exec_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	char		**argv;
	extern char	**environ;
	char		*path;

	if (cmd->type != EXEC)
	{
		error_exit("Error: exec_cmd failed\n");
	}
	ecmd = (t_execcmd *)cmd;
	if (!ecmd->word_list || !ecmd->word_list->token)
	{
		// TODO: execveを使わない時には自力でfdをクローズする必要
		exit(0);
	}
	path = search_path(ecmd->word_list->token); //未実装
	// TODO: ここで word_list を argv に変換する。仮にNULL
	argv = NULL;
	// TODO: ここ、もしくはexpandの段階でpathの解決を行う (search_path関数)
	// TODO: 環境変数はまだ実装していないから第三引数は正しくない仮
	if (execve(path, argv, environ) < 0)
	{
		perror_exit("Error: execve failed\n");
	}
	// TODO: execveが失敗すると、open on O_CLOSEXEC が機能しない
	// そのため、自力でfdをクローズする必要がある
	error_exit("Error: run_cmd failed\n");
}

void	exec_redir(t_cmd *cmd)
{
	int			oldfd;
	t_execcmd	*ecmd;
	t_redir		*redir_list;
	char		*filepath;
	int			oflag;

	if (cmd->type != EXEC)
	{
		error_exit("Error: exec_redir failed\n");
	}
	ecmd = (t_execcmd *)cmd;
	redir_list = ecmd->redir_list;
	// typeとfdが共に重複するredirectは、先頭のものしか実行されない
	// 特定のredirectを無視する仕組みが未実装
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
			perror_exit("Error: open failed\n");
		}
		if (oldfd != redir_list->fd)
		{
			if (dup2(oldfd, redir_list->fd) == -1)
			{
				perror_exit("Error: dup2 failed\n");
			}
			if (close(oldfd) == -1)
			{
				perror_exit("Error: close failed\n");
			}
		}
		redir_list = redir_list->next;
	}
}

void	run_cmd(t_cmd *cmd)
{
	if (!cmd || cmd->type == NONE)
	{
		exit(0);
	}
	else if (cmd->type == EXEC)
	{
		exec_redir(cmd);
		exec_cmd(cmd);
		// reset_fd(cmd); 未実装
	}
	else if (cmd->type == PIPE)
	{
		exec_pipe(cmd);
	}
	else
	{
		error_exit("Error: run_cmd failed\n");
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

/*

open_redir_file(); // 必要なfileをopenしたり、heredocの場合はpipeを作成したり
do_redirect();     // dupを用いて、fdのredirectを行う
exec_cmd();        // コマンドを実行する
reset_redirect();  // dupを用いて、redirectしていたfdを元に戻す

// 以下はkosekiさんから引き継いだ部分
void	validate_access(const char *path, const char *filename)
{
	if (path == NULL)
		err_exit(filename, "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(filename, "command not found", 127);
}

int	exec(char *argv[])
{
	extern char	**environ;
	const char	*path = argv[0];
	pid_t		pid;
	int			wstatus;

	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		if (strchr(path, '/') == NULL)
		{
			path = search_path(path);
		}
		validate_access(path, argv[0]);
		execve(path, argv, environ);
		fatal_error("execve");
	}
	else{
		wait(&wstatus);
		return (WEXITSTATUS(wstatus));
	}
	return (0);
}
 */
