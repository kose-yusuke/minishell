/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:17:19 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:32:51 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "utils.h"
#include "xlibc.h"

t_hash_table	*create_hash_table(void)
{
	t_hash_table	*table;

	table = malloc(sizeof(t_hash_table));
	if (!table)
		return (NULL);
	ft_bzero(table, sizeof(t_hash_table));
	table->search = search;
	table->delete = delete;
	table->insert = insert;
	table->free = free_hash_table;
	return (table);
}

static void	insert_env_var(t_hash_table *env_table, char *env)
{
	char	*delimiter;

	delimiter = ft_strchr(env, '=');
	if (delimiter)
	{
		*delimiter = '\0';
		env_table->insert(env_table, env, delimiter + 1);
	}
}

t_hash_table	*create_env_table(void)
{
	extern char		**environ;
	t_hash_table	*env_table;
	char			*env;
	size_t			i;

	env_table = create_hash_table();
	if (!env_table)
	{
		return (NULL);
	}
	i = 0;
	while (environ[i])
	{
		env = ft_strdup(environ[i]);
		if (!env)
		{
			free_hash_table(env_table);
			return (NULL);
		}
		insert_env_var(env_table, env);
		free(env);
		i++;
	}
	return (env_table);
}