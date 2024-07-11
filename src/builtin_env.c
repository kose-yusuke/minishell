/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:20:10 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/05/22 15:43:29 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv)
{
    if (argv[1])
        return (1);
	//オプションの確認
    // 全てのenvを表示
    while ()
    {

    }
    printf("_=/usr/bin/env");
	return (0);
}