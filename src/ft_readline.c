/* readline.c - 入力関連の関数 */
#include "debug_helpers.h" // TODO: remove
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

void	print_status(int status, char *location)
{
	char	*status_str;

	if (status != 0)
	{
		write(1, "==========================================", 42);
		write(1, "\n", 1);
		write(1, location, ft_strlen(location));
		status_str = ft_itoa(status);
		write(1, " : ", 3);
		write(1, status_str, ft_strlen(status_str));
		write(1, "\n", 1);
	}
}

static void	reset_status(t_mgr *mgr)
{
	if (g_status == 1)
	{
		mgr->exit_status = g_status;
		g_status = 0;
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
	print_status(g_status, "interpret"); // TODO: remove
	mgr->token = lexer(line);
	print_status(g_status, "lexer done"); // TODO: remove
	if (!mgr->token || mgr->token->type == TK_PARSE_ERROR)
	{
		report_error("lexer", "failed to tokenize", 0);
		return ;
	}
	if (peek(&(mgr->token), TK_EOF))
		return ;
	mgr->cmd = parser(mgr->token);
	print_status(g_status, "parser done"); // TODO: remove
	if (!mgr->cmd)
	{
		mgr->exit_status = 258;
		return ;
	}
	run_expansion(mgr->cmd, mgr);
	print_status(g_status, "expansion done"); // TODO: remove
	run_cmd(mgr->cmd, mgr);
	print_status(g_status, "run_cmd done"); // TODO: remove　
}

void	ft_readline(t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	init_signal();
	while (1)
	{
		print_status(g_status, "current global status");        // TODO: remove
		print_status(mgr->exit_status, "previous exit status"); // TODO: remove
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			free_mgr_resources(mgr);
			exit(255); // ？
		}
		if (*line)
		{
			add_history(line);
			interpret(line, mgr);
		}
		free(line);
		reset_status(mgr);
		reset_resources(mgr);
	}
	reset_signals();
}
