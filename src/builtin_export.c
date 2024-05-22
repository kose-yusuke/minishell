/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/05/22 15:50:54 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_all_env()
{
    // 全ての環境変数を出力する.
}

int set_key_value(char *string, char **key, char **value)
{
    char *key_end;

    key_end = ft_strchr(string, '=');
    if (key_end == NULL)
    {
        *key = string;
        *value = NULL;
        return (-1);
    }
    else
    {
        *key = ft_strndup(string, key_end - string);
        *value = ft_strdup(key_end + 1);
    }
    if (*key != NULL && *value != NULL)
        insert(t_hash_table *table, *key, *value);
    else
        return (-1);
    return (1);
}

// 返り値 : 正常が0, エラーは1
int	builtin_export(char **argv)
{
    int i;
    int status;
    char **key;
    char **value;
    
    if (argv[1] == NULL)
    {
        print_all_env();
        return (0);
    }
    i = 1;
    status = 0;
    while(argv[i])
    {
        if((set_key_value(argv[i]), &key, &value) < 0)
        {
            // エラーの場合
            builtin_error("export", argv[i], "not a valid identifier");
            status = 1;
        }
        i++; //代入される変数の分だけ繰り返す
    }
    return (status);
}