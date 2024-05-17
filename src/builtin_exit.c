/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:48:16 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/05/17 16:14:32 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_numeric(char *s)
{
	if (!ft_isdigit(*s))
		return (false);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

int builtin_exit(char **argv)
{
    char *arg;
    
    if (argv[1] == NULL)
        exit(0);
        // exit(last_status); //本当は, 引数がない場合は最後のステータスで終了する. last_statusをどうやって持ってくるか.
    if (argv[2])
    {
        perror("exit: too many arguments");
        return (1);
    }
    arg = argv[1];
    if (is_numeric(arg))
    {
        
    }
    perror("exit: numeric argument required"); // 数字でない引数の場合のエラーメッセージ
    exit(255); // エラーの場合は255で終了
}