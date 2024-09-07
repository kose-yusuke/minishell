/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:20:10 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/07 21:55:16 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv, t_mgr *mgr, int var)
{
	t_hash_table	*hash_table;
	t_hash_node		*node;
	int				i;

	i = 0;
	if (argv[1])
		return (1);
	hash_table = mgr->env_table;
	while (i < HASH_TABLE_SIZE)
	{
		node = hash_table->table[i];
		while (node)
		{
			printf("%s=%s\n", node->key, node->value);
			node = node->next;
		}
		i++;
	}
	if (var == 1)
		printf("_=/usr/bin/env\n");
	return (0);
}
