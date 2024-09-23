/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:13:13 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 03:53:57 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "utils.h"
#include "xlibc.h"

char	*get_path_value(t_env_node *env_head)
{
	t_env_node	*node;

	node = env_head;
	while (node)
	{
		if (ft_strcmp(node->key, "PATH") == 0)
		{
			return (node->value);
		}
		node = node->next;
	}
	return (NULL);
}

char	*search_env(t_env_node *env_head, char *key)
{
	t_env_node	*node;

	node = env_head;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			return (node->value);
		}
		node = node->next;
	}
	return (NULL);
}

char	*append_env(t_env_node **env_head, char *key, char *value)
{
	t_env_node	*node;
	t_env_node	*new_node;

	if (!key)
		return (NULL);
	node = *env_head;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			free(node->value);
			node->value = ft_strdup(value);
			return (node->value);
		}
		if (!node->next)
			break ;
		node = node->next;
	}
	new_node = create_env_node(key, value);
	if (!*env_head)
		*env_head = new_node;
	else
		node->next = new_node;
	return (new_node->value);
}

void	add_var_to_env_list(t_env_node **env_head, char *env_var)
{
	char	*copied_env;
	char	*eql_sign;
	char	*key;
	char	*value;

	copied_env = ft_strdup(env_var);
	if (!copied_env)
		return ;
	eql_sign = ft_strchr(copied_env, '=');
	if (eql_sign)
	{
		*eql_sign = '\0';
		key = copied_env;
		value = eql_sign + 1;
		append_env(env_head, key, value);
	}
	free(copied_env);
}

t_env_node	*create_env_list(void)
{
	extern char	**environ;
	t_env_node	*env_head;
	size_t		i;

	if (!environ)
		return (NULL);
	env_head = NULL;
	i = 0;
	while (environ[i])
	{
		add_var_to_env_list(&env_head, environ[i]);
		i++;
	}
	return (env_head);
}
