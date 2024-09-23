/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/24 03:45:38 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

// TODO: 返り値についてdelete_envとの関係をあとで確認する（sakitaha）
// 返り値 : 正常が0, エラーは1
int	builtin_unset(char **argv, t_mgr *mgr)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (argv[i])
	{
		if (delete_env(&mgr->env_list, argv[i]) == -1)
		{
			perror("unset");
			status = 1;
		}
		i++;
	}
	return (status);
}
