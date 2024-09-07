/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/07 23:15:20 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

// 返り値 : 正常が0, エラーは1
int	builtin_unset(char **argv, t_mgr *mgr)
{
	int	i;
	int	status;

	i = 1;
	
	status = 0;
	while (argv[i])
	{
		if (delete (mgr->env_table, argv[i]) < 0)
		{
			perror("unset");
			status = 1;
		}
		i++;
	}
	return (status);
}
