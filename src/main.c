/* main.c - メイン関数  */
#include "ft_readline.h"
#include "minishell.h"

static void	init_mgr(t_mgr *mgr)
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
	t_mgr	mgr;
	int		fd;

	(void)argv;
	if (argc != 1)
		error_exit("usage: ./minishell");
	// 端末デバイスファイル /dev/tty を開いて、FD 0, 1, 2 が開いていることを確認
	while (1)
	{
		fd = open("/dev/tty", O_RDWR);
		if (fd == -1)
			error_exit("fd open error for /dev/tty");
		if (fd > 2)
		{
			close(fd);
			break ;
		}
	}
	// TODO: signal handlerの実装
	init_mgr(&mgr);
	// ft_readline の中でエラーが発生した場合は、エラーメッセージを表示して終了する
	if (ft_readline(&mgr) == -1)
	{
		free_mgr_resources(&mgr);
		error_exit("ft_readline error");
	}
	// クリーンアップ処理
	free_mgr_resources(&mgr);
	return (mgr.status);
}
