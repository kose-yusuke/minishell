/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/22 02:03:13 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

// 返り値 : 正常が0, エラーは1
int	builtin_unset(char **argv, t_mgr *mgr)
{
	int i;
	int status;

	i = 1;
	while (argv[i])
	{
		if (delete (mgr->env_table, argv[i]) < 0)
		{
			// エラーの場合
			perror("unset");
			status = 1;
		}
		else
			status = 0;
		i++; //代入される変数の分だけ繰り返す
	}
	return (status);
}
