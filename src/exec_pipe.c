/* exec_pipe.c */
#include "error.h"
#include "executor.h"

static pid_t	xfork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		sys_error(NULL, "fork error");
	}
	return (pid);
}

static void	exec_leftcmd(t_pipecmd *pcmd, int pfd[2], t_mgr *mgr)
{
	// 不要なRead endを閉じる
	if (close(pfd[0]) == -1)
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
	if (pfd[1] != STDOUT_FILENO)
	{
		if (dup2(pfd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		if (close(pfd[1]) == -1)
		{
			perror("close");
			exit(EXIT_FAILURE);
		}
	}
	run_cmd(pcmd->left, mgr);
	exit(EXIT_SUCCESS);
}

static void	exec_rightcmd(t_pipecmd *pcmd, int pfd[2], t_mgr *mgr)
{
	// 不要なWrite endを閉じる
	if (close(pfd[1]) == -1)
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
	if (pfd[0] != STDIN_FILENO)
	{
		if (dup2(pfd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		if (close(pfd[0]) == -1)
		{
			perror("close");
			exit(EXIT_FAILURE);
		}
	}
	run_cmd(pcmd->right, mgr);
	exit(EXIT_SUCCESS);
}

t_status	exec_pipe(t_pipecmd *pcmd, t_mgr *mgr)
{
	int		pfd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	if (pipe(pfd) == -1)
	{
		sys_error(NULL, "pipe error");
		return (SC_FAILURE);
	}
	left_pid = xfork();
	if (left_pid < 0)
	{
		return (SC_NOEXEC); // `fork` の失敗時に `SC_NOEXEC` を返す
	}
	if (left_pid == 0)
	{
		exec_leftcmd(pcmd, pfd, mgr);
	}
	right_pid = xfork();
	if (right_pid < 0)
	{
		return (SC_NOEXEC); // `fork` の失敗時に `SC_NOEXEC` を返す
	}
	if (right_pid == 0)
	{
		exec_rightcmd(pcmd, pfd, mgr);
	}
	if (close(pfd[0]) == -1 || close(pfd[1]) == -1)
	{
		sys_error(NULL, "close error");
	}
	if (waitpid(left_pid, &left_status, 0) == -1 || waitpid(right_pid,
			&right_status, 0) == -1)
	{
		sys_error(NULL, "waitpid error");
		return (SC_FAILURE);
	}
	// 右側のプロセスがシグナルによって終了した場合、そのステータスを返す
	if (WIFSIGNALED(right_status))
		return (WTERMSIG(right_status) + 128);
	// 右側のプロセスの終了ステータスを優先して返す
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	// 左側のプロセスがシグナルによって終了した場合、そのステータスを返す
	if (WIFSIGNALED(left_status))
		return (WTERMSIG(left_status) + 128);
	// それ以外の場合は左側のプロセスの終了ステータスを返す
	if (WIFEXITED(left_status))
		return (WEXITSTATUS(left_status));
	return (SC_FAILURE);
}
