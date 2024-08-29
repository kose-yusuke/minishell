#ifndef HEREDOC_H
# define HEREDOC_H


# include "minishell.h"

# define FILE_TEMPLATE "/tmp/heredoc"
# define HEREDOC_MAX_FILES 10000

void	run_heredoc(t_cmd *cmd, t_mgr *mgr);
void	delete_tmp_files(void);
// void				ft_heredoc_output(int fd); <- heredoc関連なのでここに移動


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


#endif
