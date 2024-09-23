/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:21:13 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 03:59:26 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "xlibc.h"

t_env_node	*create_env_node(char *key, char *value)
{
	t_env_node	*new_node;

	new_node = xmalloc(sizeof(t_env_node));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

void	insert_to_sorted_list(t_env_node **sorted_head, t_env_node *node_to_add)
{
	t_env_node	*sorted_node;

	sorted_node = *sorted_head;
	if (ft_strcmp(node_to_add->key, sorted_node->key) < 0)
	{
		node_to_add->next = sorted_node;
		*sorted_head = node_to_add;
		return ;
	}
	while (sorted_node->next)
	{
		if (ft_strcmp(node_to_add->key, sorted_node->next->key) < 0)
		{
			node_to_add->next = sorted_node->next;
			sorted_node->next = node_to_add;
			return ;
		}
		sorted_node = sorted_node->next;
	}
	sorted_node->next = node_to_add;
}

t_env_node	*create_sorted_env(t_env_node *env_head)
{
	t_env_node	*sorted_head;
	t_env_node	*copied_node;
	t_env_node	*node;

	if (!env_head)
		return (NULL);
	sorted_head = create_env_node(env_head->key, env_head->value);
	node = env_head->next;
	while (node)
	{
		copied_node = create_env_node(node->key, node->value);
		insert_to_sorted_list(&sorted_head, copied_node);
		node = node->next;
	}
	return (sorted_head);
}

void	free_env_list(t_env_node *env_head)
{
	t_env_node	*node;
	t_env_node	*tmp;

	if (!env_head)
		return ;
	node = env_head;
	while (node)
	{
		tmp = node;
		node = node->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

int	delete_env(t_env_node **env_head, char *key)
{
	t_env_node	*prev;
	t_env_node	*node;

	if (!env_head || !*env_head || !key)
		return (1);
	prev = NULL;
	node = *env_head;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (prev)
				prev->next = node->next;
			else
				*env_head = node->next;
			free(node->key);
			free(node->value);
			free(node);
			return (0);
		}
		prev = node;
		node = node->next;
	}
	return (-1);
}
