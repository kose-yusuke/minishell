/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:40:05 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/11 01:02:20 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	ignore_signal(int signum)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(signum, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	restore_signal(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	if (sigaction(signum, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	restore_signals(void)
{
	rl_event_hook = NULL;
	restore_signal(SIGINT);
	restore_signal(SIGQUIT);
}

static void	setup_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_sigint;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	ignore_signal(SIGQUIT);
}

void	init_signal(void)
{
	extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = 0;
	rl_catch_signals = 0;
	if (isatty(STDIN_FILENO))
	{
		rl_event_hook = basic_sigint_hook;
	}
	setup_signals();
}

// kosekiさんから引き継いだコード

// // idle状態のminishellのカスタムシグナルハンドラ
// void	idle_handle_signal(int sig)
// {
// 	// Ctrl + C
// 	if (sig == SIGINT)
// 	{
// 		g_status = 1;
// 		write(1, "\n", 1);
// 		rl_on_new_line();       // 新しい行を表示
// 		rl_replace_line("", 0); // 入力行を空に置き換える
// 		rl_redisplay();         // 入力プロンプトを再表示
// 	}
// }

// // コマンド実行時のminishellのカスタムシグナルハンドラ
// void	exec_handle_signal(int sig)
// {
// 	// Ctrl + C
// 	if (sig == SIGINT)
// 	{
// 		// sigは2
// 		g_status = sig;
// 		write(1, "\n", 1);
// 		rl_on_new_line();       // 新しい行を表示
// 		rl_replace_line("", 0); // 入力行を空に置き換える
// 								// rl_redisplay(); // 入力プロンプトを再表示
// 	}
// 	else if (sig == SIGQUIT)
// 	{
// 		// sigは3
// 		g_status = sig;
// 		write(1, "exec SIGQUIT\n", 13);
// 		write(1, "\n", 1);
// 		rl_on_new_line();       // 新しい行を表示
// 		rl_replace_line("", 0); // 入力行を空に置き換える
// 								// rl_redisplay(); // 入力プロンプトを再表示
// 	}
// }
