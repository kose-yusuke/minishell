/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/05/17 17:09:27 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_all_env()
{
    // 全ての環境変数を出力する.
}

// 返り値 : 正常が0, エラーは1
int	builtin_export(char **argv)
{
    int i;
    
    if (argv[1] == NULL)
    {
        print_all_env();
        return (0);
    }
    i = 1;
    while(argv[i])
    {
        // if (環境変数のハッシュテーブルに追加する.)
        {
            // エラーの場合
            return (1);
        }
        i++; //代入される変数の分だけ繰り返す
    }
    return (0);
}