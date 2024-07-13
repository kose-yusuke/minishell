#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

int	ft_heredoc(const char *eof)
{
	char	*line;
	int		pfd[2];

	if (pipe(pfd) < 0)
		assert_error("pipe", "pipe");
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (strcmp(line, eof) == 0)
		{
			free(line);
			break ;
		}
		write(pfd[1], line, strlen(line));
		write(pfd[1], "\n", 1);
		free(line);
	}
	close(pfd[1]);
	return (pfd[0]);
}

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
