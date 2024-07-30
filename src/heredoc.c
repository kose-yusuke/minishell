/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:24:36 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/30 02:42:04 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	expand_heredoc(char **line, t_hash_table *env_table)
{
	char	*current_ptr;

	if (line == NULL || *line == NULL)
		return ;
	current_ptr = *line;
	while (current_ptr && *current_ptr)
	{
		expand_env(line, &current_ptr, env_table);
	}
}

// TODO: 大きな入力に対応できるようにする -> 一時ファイル
// 本家は、pipeと一時ファイルの両方を使っているらしい？（詳細は不明）
//一時的なファイルを作って, そこに書き込んでcatで出力する.ファイルは後で消す. パイプは上限があるから, 膨大な量を入力されると詰むらしい.
int	ft_heredoc(t_token *delimi_token, t_hash_table *env_table)
{
	char		*line;
	int			pfd[2];
	const char	*eof = delimi_token->word;

	if (pipe(pfd) < 0)
		perror("pipe");
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
			break ;
		if (strcmp(line, eof) == 0)
		{
			free(line);
			break ;
		}
		if (delimi_token->type == TK_WORD)
			expand_heredoc(&line, env_table);
		write(pfd[1], line, strlen(line));
		write(pfd[1], "\n", 1);
		free(line);
	}
	close(pfd[1]);
	return (pfd[0]);
}

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
