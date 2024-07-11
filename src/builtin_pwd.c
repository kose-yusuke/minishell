/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:44:10 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/05/17 17:52:17 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **argv)
{
    char	cwd_path[PATH_MAX];
    
    if (getcwd(cwd_path, PATH_MAX) == NULL)
    {
        builtin_error("pwd", NULL, "getcwd");
        return (1);
    }
    write(1, cwd_path, ft_strlen(cwd_path));
    write(1, "\n", 1);
    return (0);
}