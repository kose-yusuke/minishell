/* main.c - メイン関数  */
#include "minishell.h"

static void	mgr_init(t_mgr *mgr)
{
	memset(mgr, 0, sizeof(t_mgr));
	mgr->env_table = create_env_table();
	if (!mgr->env_table)
	{
		error_exit("failed to create env_table");
	}
}

int	main(int argc, char **argv)
{
	t_mgr			mgr;
	int				fd;

	// 端末デバイスファイル /dev/tty を開いて、FD 0, 1, 2 が開いていることを確認
	while (1)
	{
		fd = open("/dev/tty", O_RDWR);
		if (fd == -1)
		{
			error_exit("fd open error");
		}
		if (fd > 2)
		{
			close(fd);
			break ;
		}
	}
	(void)argv;
	mgr_init(&mgr);
	if (argc == 1)
	{
		ft_readline(&mgr);
	}
	return (0);
}
