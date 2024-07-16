/* executor.c - コマンドの実行とプロセス管理に関する関数の実装。 */

#include "minishell.h"

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

char	*search_path(const char *word)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
	char	*tmp_path;

	value = getenv("PATH");
	printf(value);
	while (*value)
	{
		// /bin:/usr/bin
		//     ^
		//     end
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
			tmp_path = ft_strdup(path);
			if (tmp_path == NULL)
				perror("strdup");
			return (tmp_path);
		}
		if (end == NULL)
			return (NULL);
		value = end + 1;
	}
	return (NULL);
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

static void	exec_rightcmd(t_pipecmd *pcmd, int pfd[2], t_mgr *mgr)
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
	// wait でわかる子プロセスの終了状態の管理ができていない
	if (waitpid(left_pid, NULL, 0) == -1 || waitpid(right_pid, NULL, 0) == -1)
	{
		assert_error("Error: waitpid failed\n", "exec_pipe failed\n");
	}
	exit(EXIT_SUCCESS);
}

void	exec_cmd(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	char		**argv;
	char		*path;
	extern char **environ;
	pid_t		pid;
	int i;

	if (cmd->type != EXEC)
	{
		assert_error("Error: unexpected cmd", "exec_cmd failed\n");
	}
	ecmd = (t_execcmd *)cmd;
	if (!ecmd->word_list || !ecmd->word_list->token)
	{
		// TODO: execveを使わない時には自力でfdをクローズする
		// close_fd();
		// exit(0);
		return ;
	}
	// ビルトインコマンドのチェックと実行
    if (is_builtin(ecmd)) {
        exec_builtin(ecmd, mgr);
        return;
    } else {
        path = search_path(ecmd->word_list->token->word);
        if (!path) {
            printf("Command not found: %s\n", ecmd->word_list->token->word);
            return;
        }
    }
	// TODO: ここで word_list を argv に変換する。仮にNULL
	argv = convert_list_to_array(ecmd);
	// print_argv(argv); 
	// // TODO: execveが失敗すると、open on O_CLOSEXEC が機能しない
	// // そのため、自力でfdをクローズする必要がある
	// assert_error("Error: execve failed\n", "exec_cmd failed\n");
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		// 子プロセスで実行
		if (execve(path, argv, environ) < 0)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		// 親プロセスで子プロセスの終了を待機
		if (waitpid(pid, NULL, 0) == -1)
		{
			perror("waitpid");
		}
	}
	free(path);
    // free argv
	i = 0;
    while(argv && argv[i])
	{
		free(argv[i]);
		i++;
	}
    free(argv);
}

void	run_cmd(t_cmd *cmd, t_mgr *mgr)
{
	int saved_stdin;
	int saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
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
		exec_redir(cmd, mgr);
		// if is_builtin
		// 呼ぶ built-in
		// else
		exec_cmd(cmd, mgr); // ここか、この中でbuilt-inの呼び出し
		// reset_fd(cmd); <- リソース管理
		// backup_fd(cmd); <- fdの復旧？（本来は親プロセス用）
	}
	else if (cmd->type == PIPE)
	{
		exec_pipe(cmd, mgr);
	}
	else
	{
		assert_error("Error: ", "run_cmd failed\n");
	}
	// 標準入力と標準出力を元に戻す
	if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(saved_stdin);
	close(saved_stdout);
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
