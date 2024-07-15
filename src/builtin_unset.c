/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/15 17:28:33 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// 返り値 : 正常が0, エラーは1
int	builtin_unset(char **argv, t_mgr *mgr)
{
    int i;
    int status;
    
    i = 1;
    while(argv[i])
    {
        if (delete(mgr->env_table, argv[i]) < 0)
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