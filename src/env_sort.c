/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:54:34 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 19:55:03 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "xlibc.h"

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
