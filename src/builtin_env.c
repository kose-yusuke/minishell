/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:20:10 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/15 17:25:56 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv, t_mgr *mgr, int var)
{
    t_hash_table *hash_table;
    t_hash_node *node;
    int i;

    i = 0;
    if (argv[1])
        return (1);
	//オプションの確認
    // 全てのenvを表示
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
        printf("_=/usr/bin/env");
	return (0);
}