/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:24:36 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/07 23:02:58 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

/**
 * heredoc 本文中の`$?` の展開を行う関数
 */
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

/**
 * heredoc 本文中の環境変数の展開を行う関数
 */
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
 * 一時ファイル名を生成する関数。index を用いてファイル名を作成
 */
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
 */
static char	*create_unique_tmp_file_name(void)
{
	char	*tmp_file_name;
	int		index;

	index = 0;
	while (index < HEREDOC_MAX_FILES)
	{
		tmp_file_name = generate_tmp_file_name(index);
		if (!tmp_file_name)
			return (NULL);
		if (access(tmp_file_name, F_OK) == -1) // まだ存在しないファイル名
			return (tmp_file_name);
		free(tmp_file_name);
		index++;
	}
	assert_error("too many tmp files", "create_unique_tmp_file_name");
	return (NULL);
}

/**
 * heredoc の入力を処理し、一時ファイルに書き込む関数
 */
static void	ft_heredoc(t_redir *redir, t_mgr *mgr)
{
	const char	*eof = redir->word_list->token->word;
	char		*file_name;
	int			fd;
	char		*line;

	if (!redir || !mgr)
		return ;
	file_name = create_unique_tmp_file_name();
	if (!file_name)
		return ;
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		assert_error("open failed", "open_tmpfile for write");
		free(file_name);
		return ;
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
	if (close(fd) == -1)
	{
		free(file_name);
		assert_error("close failed", "ft_heredoc");
		return ;
	}
	free(redir->word_list->token->word);
	redir->word_list->token->word = file_name;
}

/**
 * heredoc のリダイレクトをすべて処理する関数
 */
static void	process_all_heredoc(t_redir *redir, t_mgr *mgr)
{
	t_redir	*current_redir;

	current_redir = redir;
	while (current_redir)
	{
		if (current_redir->redir_type == TK_HEREDOC)
		{
			ft_heredoc(current_redir, mgr);
		}
		current_redir = current_redir->next;
	}
}

/**
 * コマンド構造体内のすべての heredoc を処理する関数
 */
void	run_heredoc(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;

	if (!cmd || cmd->type == NONE || !mgr)
		return ;
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		process_all_heredoc(ecmd->redir_list, mgr);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		run_heredoc(pcmd->left, mgr);
		run_heredoc(pcmd->right, mgr);
	}
}

/**
 *  一時ファイルの削除関数
 *  clean up系の関数をまとめた場所に移動するかもしれない
 */
void	delete_tmp_files(void)
{
	int		index;
	char	*file_name;

	index = 0;
	while (1)
	{
		file_name = generate_tmp_file_name(index);
		if (!file_name)
			break ;
		if (access(file_name, F_OK) == -1) // まだ存在しないファイル名
		{
			free(file_name);
			break ;
		}
		if (unlink(file_name) == -1)
		{
			assert_error("unlink failed", "delete_tmp_files");
			free(file_name);
			break ;
		}
		free(file_name);
		index++;
	}
}

// 以前の実装
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
