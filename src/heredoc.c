/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:24:36 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/31 04:37:46 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "heredoc.h"
#include "signals.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	process_heredoc_input(int fd, t_redir *redir, t_mgr *mgr)
{
	const char	*eof = redir->word_list->token->word;
	char		*line;

	rl_event_hook = heredoc_sigint_hook;
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
			break ;
		if (strcmp(line, eof) == 0 || g_status == 1)
		{
			free(line);
			break ;
		}
		if (redir->word_list->token->type == TK_WORD)
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
	free(redir->word_list->token->word);
	redir->word_list->token->word = file_name;
}

// 以前のheredoc実装
// int	ft_heredoc(t_token *delimi_token, t_mgr *mgr)
// {
// 	char		*line;
// 	int			pfd[2];
// 	const char	*eof = delimi_token->word;

// 	if (pipe(pfd) < 0)
// 		perror("pipe");
// 	while (1)
// 	{
// 		line = readline("heredoc> ");
// 		if (line == NULL)
// 			break ;
// 		if (strcmp(line, eof) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (delimi_token->type == TK_WORD)
// 		{
// 			expand_heredoc(&line, mgr->env_table);
// 			expand_heredoc_exit_status(&line, mgr->exit_status);
// 		}
// 		write(pfd[1], line, strlen(line));
// 		write(pfd[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pfd[1]);
// 	return (pfd[0]);
// }

// void ft_heredoc_output(int fd)
// {
// 	char buffer[1024];
// 	ssize_t bytes_read;

// 	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
// 	{
// 		buffer[bytes_read] = '\0';
// 		write(STDOUT_FILENO, buffer, bytes_read);
// 	}
// 	close(fd);
// }

// char	*expand_heredoc_line(char *line)
// {
// 	char	*new_word;
// 	char	*p;

// 	p = line;
// 	new_word = calloc(1, sizeof(char));
// 	if (new_word == NULL)
// 		fatal_error("calloc");
// 	while (*p)
// 	{
// 		if (is_variable(p))
// 			expand_variable_str(&new_word, &p, p);
// 		else if (is_special_parameter(p))
// 			expand_special_parameter_str(&new_word, &p, p);
// 		else
// 			append_char(&new_word, *p++);
// 	}
// 	free(line);
// 	return (new_word);
// }
