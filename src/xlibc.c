/* xlibc.c - 標準Cライブラリのラッパー関数 */
#include "error.h"
#include "xlibc.h"
#include <stdio.h>
#include <unistd.h>

void	*xmalloc(size_t bytes)
{
	void	*temp;

	temp = malloc(bytes);
	if (!temp)
		memory_error_and_exit("xmalloc");
	return (temp);
}

// TODO: error msgが必要か、またerrnoごとに対応を変えるか確認
int	xdup(int oldfd)
{
	int	fd;

	fd = dup(oldfd);
	if (fd == -1)
		perror("dup");
	return (fd);
}

int	xdup2(int oldfd, int newfd)
{
	newfd = dup2(oldfd, newfd);
	if (newfd == -1)
		perror("dup2");
	return (newfd);
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
- [ ] fork
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
