/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:28:09 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/10 13:20:15 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **argv)
{
	bool	echo_newline;
	size_t	i;

	echo_newline = true;
	i = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		i++;
		echo_newline = false;
	}
	if (echo_newline)
		i = 1;
	else
		i = 2;
	while (argv[i])
	{
		if (i > 1 + !echo_newline)
			write(1, " ", 1);
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
	}
	if (echo_newline)
		write(1, "\n", 1);
	return (0);
}
