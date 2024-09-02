/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 12:26:08 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/01 12:49:41 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 1;
	}
}

int	heredoc_sigint_hook(void)
{
	if (g_status == 1)
	{
		rl_done = 1;
		return (0);
	}
	return (1);
}

int	basic_sigint_hook(void)
{
	if (g_status == 1)
	{
		rl_replace_line("", 0);
		rl_done = 1;
		return (0);
	}
	return (1);
}

// idle状態の時のシグナルのセットアップ
// void	idle_setup_signals(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sa.sa_handler = idle_handle_signal;
// 	// SIGINTのハンドラを設定
// 	// Ctrl+C
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// 	sa.sa_handler = SIG_IGN;
// 	// SIGQUITのハンドラを設定
// 	// Ctrl + バックスラッシュ
// 	if (sigaction(SIGQUIT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// }

// // コマンド実行時の時のシグナルのセットアップ-親の場合はプロセスを終了しない
// void	exec_parent_setup_signals(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sa.sa_handler = exec_handle_signal;
// 	// SIGINTのハンドラを設定
// 	// Ctrl+C
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// 	sa.sa_handler = exec_handle_signal;
// 	// SIGQUITのハンドラを設定
// 	// Ctrl + バックスラッシュ
// 	if (sigaction(SIGQUIT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// }

// // コマンド実行時の時のシグナルのセットアップ-子の場合はプロセスを終了する(デフォルトの挙動)
// void	exec_child_setup_signals(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sa.sa_handler = exec_handle_signal;
// 	// SIGINTのハンドラを設定
// 	// Ctrl+C
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// 	sa.sa_handler = exec_handle_signal;
// 	// SIGQUITのハンドラを設定
// 	// Ctrl + バックスラッシュ
// 	if (sigaction(SIGQUIT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// }

// // heredoc実行時のシグナルのセットアップ-親の場合はプロセスを終了しない
// void	heredoc_parent_setup_signals(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sa.sa_handler = heredoc_handle_signal;
// 	// SIGINTのハンドラを設定
// 	// Ctrl+C
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// 	sa.sa_handler = SIG_IGN;
// 	// SIGQUITのハンドラを設定
// 	// Ctrl + バックスラッシュ
// 	if (sigaction(SIGQUIT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// }

// // heredoc実行時のシグナルのセットアップ-子の場合はプロセスを終了する(これはデフォルトの挙動)
// void	heredoc_child_setup_signals(void)
// {
// 	struct sigaction	sa;

// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	sa.sa_handler = SIG_DFL;
// 	// SIGINTのハンドラを設定
// 	// Ctrl+C
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// 	sa.sa_handler = SIG_DFL;
// 	// SIGQUITのハンドラを設定
// 	// Ctrl + バックスラッシュ
// 	if (sigaction(SIGQUIT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction");
// 		exit(EXIT_FAILURE);
// 	}
// }
