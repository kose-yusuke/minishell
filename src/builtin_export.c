/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/08 00:17:54 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

int	set_key_value(char *string, t_mgr *mgr)
{
	char	*key;
	char	*value;
	char	*key_end;
	char	*res;

	key_end = ft_strchr(string, '=');
	if (key_end == NULL)
	{
		res = mgr->env_table->insert(mgr->env_table, string, NULL);
		printf("key: %s\n", string);
		printf("res: %s\n", res);
		return (SC_SUCCESS);
	}
	key = string;
	*key_end = '\0';
	value = key_end + 1;
	// insert内でstrdupしているため、strudupする必要がない
	res = mgr->env_table->insert(mgr->env_table, key, value);
	printf("key: %s\n", string);
	printf("val: %s\n", value);
	printf("res: %s\n", res);
	return (SC_SUCCESS);
}

int	builtin_export(char **argv, t_mgr *mgr)
{
	size_t	i;
	int		status;

	if (argv[1] == NULL)
	{
		builtin_env(argv, mgr, 0);
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (set_key_value(argv[i], mgr) < 0)
		{
			perror("export");
			status = 1;
		}
		i++;
	}
	return (status);
}
