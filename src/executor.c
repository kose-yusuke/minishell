/* executor.c - コマンドの実行とプロセス管理に関する関数の実装。 */
#include "builtin_cmd.h" // convert_list_to_array
#include "error.h"
#include "executor.h"
#include "free.h"
#include "xlibc.h"

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

int	launch_command(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	char		**argv;
	char		*path;
	extern char	**environ;
	pid_t		pid;
	int			i;

	if (cmd->type != EXEC)
	{
		return (1);
	}
	ecmd = (t_execcmd *)cmd;
	if (!ecmd->word_list || !ecmd->word_list->token)
	{
		return (0);
	}
	argv = convert_list_to_array(ecmd);
	if (!argv)
		return (1);
	if (is_builtin(ecmd))
	{
		exec_builtin(ecmd, mgr);
		free_argv(argv);
		return (0);
	}
	else
	{
		path = search_path(ecmd->word_list->token->word);
		if (!path)
		{
			printf("Command not found: %s\n", ecmd->word_list->token->word);
			free_argv(argv);
			return (127);
		}
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
		// exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		// 子プロセスで実行
		if (execve(path, argv, environ) < 0)
		{
			perror("execve");
			return (1);
			// exit(EXIT_FAILURE);
		}
	}
	else
	{
		// 親プロセスで子プロセスの終了を待機
		if (waitpid(pid, NULL, 0) == -1)
		{
			perror("waitpid");
			return (1);
		}
	}
	free(path);
	free_argv(argv);
	return (0);
}

int	exec_cmd(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	int			saved_stdin;
	int			saved_stdout;
	int			saved_stderr;

	ecmd = (t_execcmd *)cmd;
	saved_stdin = xdup(STDIN_FILENO);
	saved_stdout = xdup(STDOUT_FILENO);
	saved_stderr = xdup(STDERR_FILENO);
	mgr->exit_status = exec_redir(ecmd);
	if (mgr->exit_status == SC_SUCCESS)
		mgr->exit_status = launch_command(cmd, mgr);
	xdup2(saved_stdin, STDIN_FILENO);
	xdup2(saved_stdout, STDOUT_FILENO);
	xdup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
	return (mgr->exit_status);
}

void	run_cmd(t_cmd *cmd, t_mgr *mgr)
{
	if (!cmd || !mgr || !mgr->env_table || g_status != 0)
	{
		return ;
	}
	if (cmd->type == EXEC)
	{
		mgr->exit_status = exec_cmd(cmd, mgr);
	}
	else if (cmd->type == PIPE)
	{
		// pipeのexit status　対応はあとで
		// mgr->exit_status = exec_pipe(cmd, mgr);
		exec_pipe(cmd, mgr);
	}
}
