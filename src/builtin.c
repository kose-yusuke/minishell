/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:48:26 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/13 17:55:08 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	int	a;

	a = 0;
	while (*(str + a) != '\0')
		a++;
	return (a);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(char *src)
{
	char	*p;
	long	len;

	len = ft_strlen(src);
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (!p)
		return (0);
	ft_strcpy(p, src);
	return (p);
}


char	**convert_list_to_array(t_execcmd *ecmd)
{
	char	**argv;
	t_token	*tmp_token;
	size_t	len;
	size_t	i;

	len = 0;
	tmp_token = ecmd->word_list->token;
	while (tmp_token)
	{
		tmp_token = tmp_token->next;
		len++;
	}
	if (!(argv = (char **)malloc(sizeof(char *) * (len + 1))))
		error_exit("usage: ./minishell", EXIT_FAILURE);
	tmp_token = ecmd->word_list->token;
	i = 0;
	while (tmp_token)
	{
        argv[i] = ft_strdup(tmp_token->word);
		if (!argv[i])
			error_exit("usage: ./minishell", EXIT_FAILURE);
		tmp_token = tmp_token->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

int		exec_builtin(t_execcmd *ecmd)
{
    int status;
    char	**argv;

    argv = convert_list_to_array(ecmd);
    if (strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv);
	// else if (strcmp(argv[0], "export") == 0)
	// 	status = builtin_export(argv);
	// else if (strcmp(argv[0], "unset") == 0)
	// 	status = builtin_unset(argv);
	// else if (strcmp(argv[0], "env") == 0)
	// 	status = builtin_env(argv);
	// else if (strcmp(argv[0], "cd") == 0)
	// 	status = builtin_cd(argv);
	// else if (strcmp(argv[0], "echo") == 0)
	// 	status = builtin_echo(argv);
	// else if (strcmp(argv[0], "pwd") == 0)
	// 	status = builtin_pwd(argv);
    
    return (status);
}

bool	is_builtin(t_execcmd *ecmd)
{
	const char		*cmd_name;
	char			*builtin_commands[] = {"exit", "export", "unset", "env", "cd", "echo", "pwd"};
	unsigned int	i;

	if (ecmd == NULL || ecmd->word_list == NULL || ecmd->word_list->token == NULL \
        || ecmd->word_list->token->word == NULL)
		return (false);
	cmd_name = ecmd->word_list->token->word;
	i = 0;
	while (i < sizeof(builtin_commands) / sizeof(*builtin_commands))
	{
		if (strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}