#include "minishell.h"

int	safe_dup2(int fildes, int fildes2)
{
	int	fd;

	fd = dup2(fildes, fildes2);
	if (fd == -1)
	{
		perror("dup2");
	}
	return (fd);
}
