/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:48:26 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/10/01 03:35:05 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "error.h"
#include "free.h"
#include "xlibc.h"

static bool	is_valid_arg(t_arg *arg)
{
	if (!arg || !arg->token || !arg->token->word || !arg->token->word[0])
		return (false);
	if (!is_word_or_quoted_token(arg->token))
		return (false);
	return (true);
}

static size_t	count_valid_args(t_arg *arg_list)
{
	size_t	count;
	t_arg	*tmp_arg;

	count = 0;
	tmp_arg = arg_list;
	while (tmp_arg)
	{
		if (is_valid_arg(tmp_arg))
			count++;
		tmp_arg = tmp_arg->next;
	}
	return (count);
}

char	**convert_list_to_array(t_arg *arg_list)
{
	char	**argv;
	t_arg	*tmp_arg;
	size_t	len;
	size_t	i;

	if (!arg_list)
		return (NULL);
	tmp_arg = arg_list;
	len = count_valid_args(arg_list);
	argv = (char **)xmalloc(sizeof(char *) * (len + 1));
	tmp_arg = arg_list;
	i = 0;
	while (tmp_arg)
	{
		if (is_valid_arg(tmp_arg))
			argv[i++] = ft_strdup(tmp_arg->token->word);
		tmp_arg = tmp_arg->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	exec_builtin(char **argv, t_mgr *mgr)
{
	int	status;

	status = 1;
	if (ft_strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv);
	else if (ft_strcmp(argv[0], "export") == 0)
		status = builtin_export(argv, mgr);
	else if (ft_strcmp(argv[0], "unset") == 0)
		status = builtin_unset(argv, mgr);
	else if (ft_strcmp(argv[0], "env") == 0)
		status = builtin_env(argv, mgr, 1);
	else if (ft_strcmp(argv[0], "cd") == 0)
		status = builtin_cd(argv, mgr);
	else if (ft_strcmp(argv[0], "echo") == 0)
		status = builtin_echo(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
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
		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}
