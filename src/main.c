/* main.c - メイン関数  */
#include "minishell.h"

static void	mgr_init(t_mgr *mgr)
{
	mgr->status = 0;
	mgr->syntax_error = false;
}

int	main(int argc, char **argv)
{
	t_mgr		mgr;
	extern char	**environ;
	int			fd;

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
	// TODO: environの処理（必要であれば）<- 他の場所かも
	// TODO: signalの設定
	// init_sigaction(server_signal_action);
	if (argc == 1)
	{
		input(environ, &mgr);
	}
	return (0);
}
