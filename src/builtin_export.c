/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/24 04:04:17 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

void	display_sorted_env(t_env_node *env_list)
{
	t_env_node	*sorted_env_head;
	t_env_node	*sorted_env;

	sorted_env_head = create_sorted_env(env_list);
	sorted_env = sorted_env_head;
	while (sorted_env)
	{
		printf("declare -x %s=\"%s\"\n", sorted_env->key, sorted_env->value);
		sorted_env = sorted_env->next;
	}
	free_env_list(sorted_env_head);
}

int	set_key_value(char *string, char **key, char **value, t_mgr *mgr)
{
	char	*key_end;

	key_end = ft_strchr(string, '=');
	if (key_end == NULL)
	{
		*key = ft_strdup(string);
		*value = NULL;
	}
	else
	{
		*key = ft_strndup(string, key_end - string);
		*value = ft_strdup(key_end + 1);
	}
	if (*key != NULL)
	{
		append_env(&mgr->env_list, *key, *value);
		free(*key);
		if (*value)
			free(*value);
		return (1);
	}
	if (*key)
		free(*key);
	return (-1);
}

int	builtin_export(char **argv, t_mgr *mgr)
{
	int		i;
	int		status;
	char	*key;
	char	*value;

	if (argv[1] == NULL)
	{
		display_sorted_env(mgr->env_list);
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (set_key_value(argv[i], &key, &value, mgr) < 0)
		{
			perror("export");
			status = 1;
		}
		i++;
	}
	return (status);
}
