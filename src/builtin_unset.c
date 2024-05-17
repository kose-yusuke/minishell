/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/05/17 17:15:26 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// 返り値 : 正常が0, エラーは1
int	builtin_unset(char **argv)
{
    int i;
    
    i = 1;
    while(argv[i])
    {
        // if (環境変数のハッシュテーブルから削除する.)
        {
            // エラーの場合
            return (1);
        }
        i++; //代入される変数の分だけ繰り返す
    }
    return (0);
}