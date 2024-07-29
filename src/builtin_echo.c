/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:28:09 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/26 11:23:09 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **argv)
{
	bool	is_first_arg;
	bool	echo_newline;
	size_t	i;

	echo_newline = true;
	// -nオプションのみ実装. このオプションがあるときは改行しない.
	if (argv[1] && strncmp(argv[1], "-n", 2) == 0)
	{
		i++;
		echo_newline = false;
	}
	is_first_arg = true;
	if (echo_newline)
		i = 1;
	else
		i = 2;
	while (argv[i])
	{
		if (!is_first_arg)
			write(1, " ", 1);
		is_first_arg = false;
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
	}
	if (echo_newline)
		write(1, "\n", 1);
	return (0);
}
