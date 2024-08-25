/* readline.c - 入力関連の関数 */
#include "error.h"
#include "executor.h"
#include "expander.h"
#include "free.h"
#include "ft_readline.h"
#include "heredoc.h"
#include "lexer.h"
#include "parser.h"
#include "signals.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	reset_status(t_mgr *mgr)
{
	if (g_signal_status == 1)
	{
		mgr->exit_status = g_signal_status;
		g_signal_status = 0;
	}
}

static void	reset_resources(t_mgr *mgr)
{
	delete_tmp_files();
	free_tokens(mgr->token);
	free_cmd(mgr->cmd);
	mgr->token = NULL;
	mgr->cmd = NULL;
}

static void	interpret(char *line, t_mgr *mgr)
{
	// if (g_signal_status == 1)
	// 	mgr->exit_status = g_signal_status;
	// g_signal_status = 0;
	// exec_parent_setup_signals();
	mgr->token = lexer(line);
	if (!mgr->token || mgr->token->type == TK_PARSE_ERROR)
	{
		report_error("lexer", "failed to tokenize", 0);
		return ;
	}
	if (peek(&(mgr->token), TK_EOF))
		return ;
	mgr->cmd = parser(mgr->token);
	if (!mgr->cmd)
	{
		mgr->exit_status = 258;
		return ;
	}
	run_expansion(mgr->cmd, mgr->env_table);
	run_heredoc(mgr->cmd, mgr);
	run_cmd(mgr->cmd, mgr);
}

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = 1;
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_sigint;
	// SIGINTのハンドラを設定 Ctrl+C を受け取る
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	sa.sa_handler = SIG_IGN;
	// SIGQUITのハンドラを設定 Ctrl + \ を無視
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

static int	basic_sigint_hook(void)
{
	if (g_signal_status == 1)
	{
		rl_replace_line("", 0);
		rl_done = 1;
		g_signal_status = 0;
		return (0);
	}
	return (1);
}

void	init_signal(void)
{
	extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = 0; // 制御文字^Cを表示しない
	rl_catch_signals = 0;       // Ctrl + C で終了しない
	if (isatty(STDIN_FILENO))
		rl_event_hook = basic_sigint_hook;
	setup_signals();
}

void	ft_readline(t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	init_signal();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			free_mgr_resources(mgr);
			exit(255); // exit statusは何で返すのか？
		}
		if (*line)
		{
			if (mgr->exit_status == 0)
				write(1, "healthy\n", 8);
			add_history(line);
			interpret(line, mgr);
		}
		free(line);
		reset_status(mgr);
		reset_resources(mgr);
	}
	reset_signals();
}
