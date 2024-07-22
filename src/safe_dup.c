#include "minishell.h"

int	safe_dup(int fildes)
{
	int	fd;

	errno = 0;
	fd = dup(fildes);
	if (fd == -1)
	{
		perror("dup");
	}
	return (fd);
}
