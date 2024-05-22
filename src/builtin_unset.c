/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/05/22 15:27:46 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// 返り値 : 正常が0, エラーは1
int	builtin_unset(char **argv)
{
    int i;
    int status;
    
    i = 1;
    while(argv[i])
    {
        if (delete(t_hash_table *table, argv[i]) < 0)
        {
            // エラーの場合
            builtin_error("unset", argv[i], "not a valid identifier");
            status = 1;
        }
        else
            status = 0;
        i++; //代入される変数の分だけ繰り返す
    }
    return (status);
}