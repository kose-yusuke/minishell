/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:21:13 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/30 16:40:43 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "xlibc.h"

char	*get_env(t_env_node *env_head, char *key)
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

static bool	is_valid_key(char *key)
{
	if (!key || !*key || ft_strchr(key, '='))
		return (false);
	if (!ft_isalpha(*key) && *key != '_')
		return (false);
	while (*key)
	{
		if (!ft_isalnum(*key) && *key != '_')
			return (false);
		key++;
	}
	return (true);
}

int	set_env(t_env_node **env_head, char *key, char *value)
{
	t_env_node	*node;

	if (!is_valid_key(key))
		return (-1);
	node = *env_head;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (node->value)
			{
				free(node->value);
				node->value = ft_strdup(value);
			}
			return (0);
		}
		if (!node->next)
			break ;
		node = node->next;
	}
	if (!*env_head)
		*env_head = create_env_node(key, value);
	else
		node->next = create_env_node(key, value);
	return (0);
}

int	unset_env_head(t_env_node **env_head, char *key)
{
	t_env_node	*node;

	node = *env_head;
	if (ft_strcmp(node->key, key) == 0)
	{
		*env_head = node->next;
		free(node->key);
		free(node->value);
		free(node);
		return (0);
	}
	return (-1);
}

int	unset_env(t_env_node **env_head, char *key)
{
	t_env_node	*node;
	t_env_node	*tmp;

	if (!env_head || !*env_head)
		return (1);
	if (!is_valid_key(key))
		return (-1);
	if (unset_env_head(env_head, key) == 0)
		return (0);
	node = *env_head;
	while (node && node->next)
	{
		if (ft_strcmp(node->next->key, key) == 0)
		{
			tmp = node->next;
			node->next = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return (0);
		}
		node = node->next;
	}
	return (1);
}
