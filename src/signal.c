/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:40:05 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/22 14:21:53 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <readline/history.h>
#include <readline/readline.h>

// idle状態のminishellのカスタムシグナルハンドラ
void	idle_handle_signal(int sig)
{
	// Ctrl + C
	if (sig == SIGINT)
	{
		g_signal_status = 1;
		write(1, "\n", 1);
		rl_on_new_line();       // 新しい行を表示
		rl_replace_line("", 0); // 入力行を空に置き換える
		rl_redisplay();         // 入力プロンプトを再表示
	}
}

// コマンド実行時のminishellのカスタムシグナルハンドラ
void	exec_handle_signal(int sig)
{
	// Ctrl + C
	if (sig == SIGINT)
	{
		// sigは2
		g_signal_status = sig;
		write(1, "\n", 1);
		rl_on_new_line();       // 新しい行を表示
		rl_replace_line("", 0); // 入力行を空に置き換える
								// rl_redisplay(); // 入力プロンプトを再表示
	}
	else if (sig == SIGQUIT)
	{
		// sigは3
		g_signal_status = sig;
		write(1, "Quit: 3\n", 8);
		rl_on_new_line();       // 新しい行を表示
		rl_replace_line("", 0); // 入力行を空に置き換える
								// rl_redisplay(); // 入力プロンプトを再表示
	}
}

// heredoc実行時のminishellのカスタムシグナルハンドラ
void	heredoc_handle_signal(int sig)
{
	// Ctrl + C
	if (sig == SIGINT)
	{
		g_signal_status = 1;
		write(1, "\n", 1);
		rl_on_new_line();       // 新しい行を表示
		rl_replace_line("", 0); // 入力行を空に置き換える
		rl_redisplay();         // 入力プロンプトを再表示
	}
}

// シグナルのリセット
void	reset_signal(int signum)
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

void	reset_signals(void)
{
	reset_signal(SIGINT);
	reset_signal(SIGQUIT);
}
