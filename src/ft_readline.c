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
	char	*g_status_str;
	char	*mgr_status_str;

	if (1)
		return ;
	if (g_status != 0 || status != 0)
	{
		write(1, "\n\n", 2);
		write(1, "<<", 2);
		write(1, location, ft_strlen(location));
		write(1, ">>", 2);
		g_status_str = ft_itoa(g_status);
		mgr_status_str = ft_itoa(status);
		write(1, " g_status : ", 12);
		write(1, g_status_str, ft_strlen(g_status_str));
		write(1, ", mgr_status : ", 15);
		write(1, mgr_status_str, ft_strlen(mgr_status_str));
		write(1, "\n\n", 2);
		free(mgr_status_str);
		free(g_status_str);
	}
}

static void	reset_status(t_mgr *mgr)
{
	if (mgr->exit_status == 0)
	{
		mgr->exit_status = g_status;
	}
	g_status = 0;
}

static void	reset_resources(t_mgr *mgr)
{
	delete_tmp_files();
	free_tokens(mgr->token);
	free_cmd(mgr->cmd);
	mgr->token = NULL;
	mgr->cmd = NULL;
}

static bool	has_content(char *line)
{
	while (*line && ft_strchr(" \t", *line))
		line++;
	return (*line != '\0');
}

static void	interpret(char *line, t_mgr *mgr)
{
	print_status(mgr->exit_status, "interpret start"); // TODO: remove
	mgr->token = lexer(line);
	print_status(mgr->exit_status, "lexer done"); // TODO: remove
	if (!mgr->token || mgr->token->type == TK_PARSE_ERROR)
	{
		// exit status は ?
		report_error("lexer", "failed to tokenize", 0);
		return ;
	}
	mgr->cmd = parser(mgr->token);
	print_status(mgr->exit_status, "parser done"); // TODO: remove
	if (!mgr->cmd)
	{
		mgr->exit_status = 258;
		return ;
	}
	run_expansion(mgr->cmd, mgr);
	print_status(mgr->exit_status, "expansion done"); // TODO: remove
	mgr->exit_status = run_cmd(mgr->cmd, mgr);
	print_status(mgr->exit_status, "run command done"); // TODO: remove
}

#include <unistd.h> // for getpid

void	check_for_leaks(void)
{
	pid_t	pid;
	char	command[256];

	pid = getpid();
	snprintf(command, sizeof(command), "leaks -q %d", pid);
	system(command);
}

void	ft_readline(t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	init_signal();
	while (1)
	{
		print_status(mgr->exit_status, "ready to read");
		// TODO: remove
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			free_mgr_resources(mgr);
			exit(255); // ？
		}
		if (*line && has_content(line))
		{
			add_history(line);
			interpret(line, mgr);
		}
		free(line);
		reset_status(mgr);
		reset_resources(mgr);
		// check_for_leaks();
		// system("leaks -q minishell"); // TODO: remove
	}
	restore_signals();
}
