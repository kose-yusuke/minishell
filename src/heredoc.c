/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:24:36 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/07 16:16:49 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FILE_TEMPLATE "/tmp/heredoc"

// #define LOCK_FILE "/.tmp/heredoc.lock" （pipeとheredocが同時に使われるときのためにあとで実装する）

static void	expand_heredoc_exit_status(char **line, int exit_status)
{
	char	*current_ptr;

	if (line == NULL || *line == NULL)
		return ;
	current_ptr = *line;
	while (current_ptr && *current_ptr)
	{
		expand_exit_status(line, &current_ptr, exit_status);
	}
}

static void	expand_heredoc(char **line, t_hash_table *env_table)
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

/**
 * 一時ファイル名を生成するための関数。index を用いてファイル名を作成
 * */
static char	*generate_tmp_file_name(int index)
{
	char	*index_str;
	char	*tmp_file_name;

	index_str = ft_itoa(index);
	if (!index_str)
	{
		assert_error("ft_itoa failed", "generate_tmp_file_name");
		return (NULL);
	}
	tmp_file_name = malloc(ft_strlen(FILE_TEMPLATE) + ft_strlen(index_str) + 1);
	if (!tmp_file_name)
	{
		free(index_str);
		assert_error("malloc failed", "generate_tmp_file_name");
		return (NULL);
	}
	strcpy(tmp_file_name, FILE_TEMPLATE);
	strcat(tmp_file_name, index_str);
	free(index_str);
	return (tmp_file_name);
}

/**
 * 存在しない一意な一時ファイル名を生成する関数
 * */
static char	*create_unique_tmp_file_name(void)
{
	char	*tmp_file_name;
	int		index;

	index = 0;
	while (true)
	{
		tmp_file_name = generate_tmp_file_name(index);
		if (!tmp_file_name)
			return (NULL);
		if (access(tmp_file_name, F_OK) == -1) // まだ存在しないファイル名
			return (tmp_file_name);
		free(tmp_file_name);
		index++;
		if (index >= INT_MAX)
		{
			assert_error("too many tmp files", "create_unique_tmp_file_name");
			return (NULL);
		}
	}
}

/**
 * heredoc の入力を処理し、一時ファイルに書き込む関数
 * */
int	ft_heredoc(t_redir *redir, t_mgr *mgr)
{
	char		*line;
	const char	*eof = redir->word_list->token->word;
	int			fd;
	char		*file_name;

	file_name = create_unique_tmp_file_name();
	// permission 0644で大丈夫?
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		assert_error("open failed", "open_tmpfile for write");
		return (-1);
	}
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
		if (redir->word_list->token->type == TK_WORD)
		{
			expand_heredoc(&line, mgr->env_table);
			expand_heredoc_exit_status(&line, mgr->status);
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	fd = open(generate_tmp_file_name(0), O_RDONLY);
	if (fd < 0)
	{
		assert_error("open failed", "open_tmpfile for read");
		return (-1);
	}
	return (fd);
}

/**
 *  一時ファイルの削除関数 <- 1行処理ごとに一括でcleanupする
 * */
void	delete_tmp_files(void)
{
	int		index;
	char	*file_name;

	index = 0;
	while (true)
	{
		file_name = generate_tmp_file_name(index);
		if (!file_name)
			break ;
		if (access(file_name, F_OK) == -1) // まだ存在しないファイル名
		{
			free(file_name);
			break ;
		}
		unlink(file_name); // 一時ファイルを削除
		free(file_name);
		index++;
	}
}

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
// 			expand_heredoc_exit_status(&line, mgr->status);
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
