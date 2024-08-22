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
	if (g_signal_status == 1)
		mgr->exit_status = g_signal_status;
	g_signal_status = 0;
	exec_parent_setup_signals();
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

//一周でなぜか終わる
void	ft_readline(t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	idle_setup_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			free_mgr_resources(mgr);
			// exit statusは何で返すのか？
			exit(255);
		}
		if (*line)
		{
			add_history(line);
			interpret(line, mgr);
		}
		free(line);
		reset_resources(mgr);
	}
}
