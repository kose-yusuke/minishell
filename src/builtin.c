/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:48:26 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/07 22:02:40 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "error.h"
#include "free.h"
#include "xlibc.h"

void	print_argv(char **argv)
{
	int	i;

	i = 0;
	if (argv == NULL)
	{
		printf("argv is NULL\n");
		return ;
	}
	printf("argv:\n");
	while (argv[i])
	{
		if (argv[i] == NULL)
		{
			printf("argv[%d] is NULL\n", i);
		}
		else
		{
			printf("argv[%d]: %s\n", i, argv[i]);
		}
		i++;
	}
}

char	**convert_list_to_array(t_word *word_list)
{
	char	**argv;
	t_word	*tmp_word;
	size_t	len;
	size_t	i;

	if (!word_list || !word_list->token || !word_list->token->word)
		return (NULL);
	len = 0;
	tmp_word = word_list;
	while (tmp_word)
	{
		tmp_word = tmp_word->next;
		len++;
	}
	argv = (char **)xmalloc(sizeof(char *) * (len + 1));
	tmp_word = word_list;
	i = 0;
	while (tmp_word && is_word_or_quoted_token(tmp_word->token))
	{
		argv[i++] = ft_strdup(tmp_word->token->word);
		tmp_word = tmp_word->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	exec_builtin(char **argv, t_mgr *mgr)
{
	int	status;

	if (strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv);
	else if (strcmp(argv[0], "export") == 0)
		status = builtin_export(argv, mgr);
	else if (strcmp(argv[0], "unset") == 0)
		status = builtin_unset(argv, mgr);
	else if (strcmp(argv[0], "env") == 0)
		status = builtin_env(argv, mgr, 1);
	else if (strcmp(argv[0], "cd") == 0)
		status = builtin_cd(argv, mgr);
	else if (strcmp(argv[0], "echo") == 0)
		status = builtin_echo(argv);
	else if (strcmp(argv[0], "pwd") == 0)
		status = builtin_pwd(argv);
	return (status);
}

bool	is_builtin(char *cmd_name)
{
	const char		*builtin_commands[] = {"exit", "export", "unset", "env",
		"cd", "echo", "pwd"};
	unsigned int	i;

	if (!cmd_name)
		return (false);
	i = 0;
	while (i < sizeof(builtin_commands) / sizeof(*builtin_commands))
	{
		if (strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}
