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

void	exec_pipe(t_cmd *cmd, t_mgr *mgr)
{
	t_pipecmd	*pcmd;
	int			pfd[2];
	pid_t		left_pid;
	pid_t		right_pid;
	int			left_status;
	int			right_status;

	if (cmd->type != PIPE)
	{
		assert_error("Error: unexpected cmd", "exec_pipe failed\n");
		return ;
	}
	pcmd = (t_pipecmd *)cmd;
	if (pipe(pfd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	left_pid = fork_pid();
	if (left_pid == 0)
	{
		exec_leftcmd(pcmd, pfd, mgr);
		exit(EXIT_SUCCESS);
	}
	right_pid = fork_pid();
	if (right_pid == 0)
	{
		exec_rightcmd(pcmd, pfd, mgr);
		exit(EXIT_SUCCESS);
	}
	if (close(pfd[0]) == -1 || close(pfd[1]) == -1)
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
	if (waitpid(left_pid, &left_status, 0) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if (waitpid(right_pid, &right_status, 0) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if (WIFEXITED(left_status))
	{
		printf("Left child (PID %d) exited with status %d\n", left_pid,
			WEXITSTATUS(left_status));
	}
	else if (WIFSIGNALED(left_status))
	{
		printf("Left child (PID %d) killed by signal %d\n", left_pid,
			WTERMSIG(left_status));
	}
	if (WIFEXITED(right_status))
	{
		printf("Right child (PID %d) exited with status %d\n", right_pid,
			WEXITSTATUS(right_status));
	}
	else if (WIFSIGNALED(right_status))
	{
		printf("Right child (PID %d) killed by signal %d\n", right_pid,
			WTERMSIG(right_status));
	}
}
