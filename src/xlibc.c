/* xlibc.c - 標準Cライブラリのラッパー関数 */
#include "error.h"
#include "xlibc.h"
#include <stdio.h>
#include <unistd.h>

/*
error.h にあるmemory_error_and_exit関数の内容
void	memory_error_and_exit(char *func_name)
{
	write(2, func_name, ft_strlen(func_name));
	write(2, ": out of virtual memory\n", 24);
	exit(SC_FATAL_ERROR);
}
*/

void	*xmalloc(size_t bytes)
{
	void	*temp;

	temp = malloc(bytes);
	if (!temp)
		memory_error_and_exit("xmalloc");
	return (temp);
}

int	xclose(int fd)
{
	if (close(fd) == -1)
	{
		sys_error("minishell", "close");
		return (-1);
	}
	return (0);
}

int	xdup(int oldfd)
{
	int	fd;

	fd = dup(oldfd);
	if (fd == -1)
		sys_error("minishell", "dup");
	return (fd);
}

int	xdup2(int oldfd, int newfd)
{
	newfd = dup2(oldfd, newfd);
	if (newfd == -1)
		sys_error("minishell", "dup2");
	return (newfd);
}

pid_t	xfork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		sys_error("minishell", "fork");
	return (pid);
}

/*
note:
minishellの使用許可内かつ`xlibc.c` に収めても違和感のない関数
必要に応じて、ラッパー関数を作成する

- [x] malloc (xmalloc)
- [ ] free
- [ ] open
- [ ] close
- [ ] read
- [ ] write
- [x] dup (xdup)
- [x] dup2 (xdup2)
- [x] fork
- [ ] pipe
- [ ] wait
- [ ] waitpid
- [ ] execve
- [ ] access
- [ ] getcwd
- [ ] chdir
- [ ] unlink
- [ ] opendir
- [ ] readdir
- [ ] closedir
 */
