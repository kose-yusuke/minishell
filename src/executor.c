/* executor.c - コマンドの実行とプロセス管理に関する関数の実装。 */

#include "executor.h"
#include "minishell.h"



char	*search_path(const char *word)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
	char	*tmp_path;

	value = getenv("PATH");
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

void	exec_cmd(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	char		**argv;
	char		*path;
	extern char	**environ;
	pid_t		pid;
	int			i;

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
	argv = convert_list_to_array(ecmd);
	if(!argv)
		return ;
	// print_argv(argv);
	// ビルトインコマンドのチェックと実行
    if (is_builtin(ecmd)) {
        exec_builtin(ecmd, mgr);
		// system("leaks -q minishell");
		free_argv(argv);
        return;
    } 
	else 
	{
        path = search_path(ecmd->word_list->token->word);
        if (!path) 
		{
            printf("Command not found: %s\n", ecmd->word_list->token->word);
			free_argv(argv);
            return;
        }
    }
	// TODO: パイプやリダイレクト以下の文字列も引数として含めてしまっているため, 少し処理を変える必要あり. 
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
	while (argv && argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	run_cmd(t_cmd *cmd, t_mgr *mgr)
{
	int			saved_stdin;
	int			saved_stdout;
	t_execcmd	*ecmd;

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
		ecmd = (t_execcmd *)cmd;
		exec_redir(ecmd->redir_list, mgr); // TODO: 呼び出し位置をあとで考える
		exec_cmd(cmd, mgr);                // ここか、この中でbuilt-inの呼び出し
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
	if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout,
			STDOUT_FILENO) == -1)
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
