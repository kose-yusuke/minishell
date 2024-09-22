/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:16:55 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 01:28:22 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "free.h"
#include "ft_readline.h"
#include "minishell.h"
#include <libc.h>

// __attribute__((destructor)) static void destructor()
// {
// 	system("leaks -q minishell");
// }

volatile sig_atomic_t	g_status = 0;

static void	init_mgr(t_mgr *mgr)
{
	memset(mgr, 0, sizeof(t_mgr));
	mgr->env_table = create_env_table();
	if (!mgr->env_table)
		error_exit("failed to create env_table", EXIT_FAILURE);
	g_status = 0;
}

int	main(int argc, char **argv)
{
	t_mgr	mgr;
	int		fd;

	(void)argv;
	if (argc != 1)
		error_exit("usage: ./minishell", EXIT_FAILURE);
	while (1)
	{
		fd = open("/dev/tty", O_RDWR);
		if (fd == -1)
			error_exit("fd open error for /dev/tty", EXIT_FAILURE);
		if (fd > 2)
		{
			close(fd);
			break ;
		}
	}
	init_mgr(&mgr);
	ft_readline(&mgr);
	free_mgr_resources(&mgr);
	return (0);
}
