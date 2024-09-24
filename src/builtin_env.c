/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:20:10 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/24 15:07:22 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv, t_mgr *mgr, int var)
{
	t_env_node	*env_node;

	if (argv[1])
		return (1);
	env_node = mgr->env_list;
	while (env_node)
	{
		if (env_node->key && env_node->value)
			printf("%s=%s\n", env_node->key, env_node->value);
		env_node = env_node->next;
	}
	if (var == 1)
		printf("_=/usr/bin/env\n");
	return (0);
}
