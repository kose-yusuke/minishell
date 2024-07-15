/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/15 17:27:56 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		while (*s == (char)c)
		{
			return ((char *)s);
		}
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

int set_key_value(char *string, char **key, char **value, t_mgr *mgr)
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
        insert(mgr->env_table, *key, *value);
    else
        return (-1);
    return (1);
}

// 返り値 : 正常が0, エラーは1
int	builtin_export(char **argv, t_mgr *mgr)
{
    int i;
    int status;
    char **key;
    char **value;
    
    if (argv[1] == NULL)
    {
        builtin_env(argv,mgr,0);
        return (0);
    }
    i = 1;
    status = 0;
    while(argv[i])
    {
        if(set_key_value(argv[i], key, value, mgr) < 0)
        {
            // エラーの場合
            perror("export");
            status = 1;
        }
        i++; //代入される変数の分だけ繰り返す
    }
    return (status);
}