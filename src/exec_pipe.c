/* exec_pipe.c */
#include "error.h"
#include "executor.h"
#include "xlibc.h"

static int	wait_for_children(int pfd[2], pid_t left_pid, pid_t right_pid)
{
	int	status;

	xclose(pfd[0]);
	xclose(pfd[1]);
	if (waitpid(left_pid, 0, 0) == -1 || waitpid(right_pid, &status, 0) == -1)
	{
		sys_error(NULL, "waitpid error");
		return (-1);
	}
	return (status);
}

static t_status	eval_exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (SC_FAILURE);
}

t_status	exec_pipe(t_pipecmd *pcmd, t_mgr *mgr)
{
	int		pfd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (pipe(pfd) == -1)
	{
		sys_error("minishell", "pipe");
		return (SC_FAILURE);
	}
	left_pid = create_left_process(pcmd->left, pfd, mgr);
	if (left_pid == -1)
		return (SC_NOEXEC);
	right_pid = create_right_process(pcmd->right, pfd, mgr);
	if (right_pid == -1)
		return (SC_NOEXEC);
	status = wait_for_children(pfd, left_pid, right_pid);
	if (status == -1)
		return (SC_FAILURE);
	return (eval_exit_status(status));
}
