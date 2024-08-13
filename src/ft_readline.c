/* readline.c - 入力関連の関数 */
#include "ft_readline.h"
#include "minishell.h"


void	reset_resources(t_mgr *mgr)
{
	if (mgr->status == 0 && mgr->syntax_error) // これなんだっけ？
		mgr->status = 1;
	delete_tmp_files();
	free_tokens(mgr->token);
	free_cmd(mgr->cmd);
	mgr->token = NULL;
	mgr->cmd = NULL;
}

void	interpret(char *line, t_mgr *mgr)
{
	if (g_status == 130 || g_status == 131)
		mgr->status = g_status;
	g_status = 0;
	mgr->token = lexer(line);
	if (!mgr->token || mgr->token->type == TK_PARSE_ERROR)
	{
		report_error("lexer error", 0, 0); // ?
		return ;
	}
	if (mgr->token->type == TK_EOF)
	{
		return ;
	}
	// print_tokens(mgr->token); // debug
	mgr->cmd = parser(mgr->token);
	if (!mgr->cmd || mgr->cmd->type == NONE)
	{
		report_error("parser error", 0, 0); // ?
		return ;
	}
	// print_cmd(mgr->cmd); // デバッグ用の出力
	run_expansion(mgr->cmd, mgr->env_table);
	run_heredoc(mgr->cmd, mgr);
	run_cmd(mgr->cmd, mgr);
}

//一周でなぜか終わる
void	ft_readline(t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	mgr->status = 0; // ?
	setup_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			free_mgr_resources(mgr);
			exit(0);
			// error_exit("failed to read line", EXIT_FAILURE); // ?
			// break ;
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
