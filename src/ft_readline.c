/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:07:16 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 04:09:26 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	mgr->token = lexer(line);
	if (!mgr->token || mgr->token->type == TK_PARSE_ERROR)
	{
		mgr->exit_status = 258;
		report_error("lexer", "failed to tokenize", 0);
		return ;
	}
	mgr->cmd = parser(mgr->token);
	if (!mgr->cmd)
	{
		mgr->exit_status = 258;
		return ;
	}
	run_expansion(mgr->cmd, mgr);
	mgr->exit_status = run_cmd(mgr->cmd, mgr);
}

// #include <unistd.h> // for getpid

// void	check_for_leaks(void)
// {
// 	pid_t	pid;
// 	char	command[256];

// 	pid = getpid();
// 	snprintf(command, sizeof(command), "leaks -q %d", pid);
// 	system(command);
// }

// check_for_leaks();
// system("leaks -q minishell"); // TODO: remove

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
			write(1, "exit\n", 5);
			free_mgr_resources(mgr);
			exit(255);
		}
		if (*line && has_content(line))
		{
			add_history(line);
			interpret(line, mgr);
		}
		free(line);
		reset_status(mgr);
		reset_resources(mgr);
	}
	restore_signals();
}
