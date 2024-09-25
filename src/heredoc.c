/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:24:36 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/25 01:10:38 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "heredoc.h"
#include "signals.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	process_heredoc_input(int fd, t_redir *redir, t_mgr *mgr)
{
	const char	*eof = redir->arg_list->token->word;
	char		*line;

	rl_event_hook = heredoc_sigint_hook;
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, eof) == 0 || g_status == 1)
		{
			free(line);
			break ;
		}
		if (redir->arg_list->token->type == TK_WORD)
			expand_word_str(&line, mgr);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	rl_event_hook = basic_sigint_hook;
}

void	ft_heredoc(t_redir *redir, t_mgr *mgr)
{
	char	*file_name;
	int		fd;

	if (!redir || !mgr)
		return ;
	fd = prepare_tmp_file(&file_name);
	if (fd == -1)
		return ;
	process_heredoc_input(fd, redir, mgr);
	if (close(fd) == -1)
	{
		free(file_name);
		sys_error("heredoc", "close");
		return ;
	}
	free(redir->arg_list->token->word);
	redir->arg_list->token->word = file_name;
}
