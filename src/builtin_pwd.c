/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:44:10 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/22 13:17:23 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

int	builtin_pwd(char **argv)
{
	char	cwd_path[PATH_MAX];

	if (getcwd(cwd_path, PATH_MAX) == NULL)
	{
		perror("pwd");
		return (1);
	}
	write(1, cwd_path, ft_strlen(cwd_path));
	write(1, "\n", 1);
	return (0);
}
