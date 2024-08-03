/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:40:05 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/03 17:51:30 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

// シグナルハンドラ
void handle_signal(int sig)
{
    //Ctrl + C
    if (sig == SIGINT)
    {
        printf("\n");
        rl_on_new_line(); // 新しい行を表示
        rl_replace_line("", 0); // 入力行を空に置き換える
        rl_redisplay(); // 入力プロンプトを再表示
    }
    else if (sig == SIGQUIT)
    {
        printf("Quit\n");
    }
}

// シグナルのセットアップ
void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    // SIGINTのハンドラを設定
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

        // SIGQUITのハンドラを設定
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

// シグナルのリセット
void reset_signal(int signum)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_DFL;
    if (sigaction(signum, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

void reset_signals(void)
{
    reset_signal(SIGINT);
    reset_signal(SIGQUIT);
}
