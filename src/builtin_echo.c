/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:28:09 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/10/01 04:53:30 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_echo_n_option(char *arg)
{
	if (!arg || ft_strlen(arg) < 2 || arg[0] != '-' || arg[1] != 'n')
		return (false);
	arg++;
	while (*arg == 'n')
		arg++;
	return (*arg == '\0');
}

int	builtin_echo(char **argv)
{
	bool	echo_newline;
	bool	is_first_arg_to_print;
	size_t	i;

	echo_newline = true;
	i = 1;
	while (argv[i] && is_echo_n_option(argv[i]))
	{
		echo_newline = false;
		i++;
	}
	is_first_arg_to_print = true;
	while (argv[i])
	{
		if (is_first_arg_to_print)
			is_first_arg_to_print = false;
		else
			write(1, " ", 1);
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
	}
	if (echo_newline)
		write(1, "\n", 1);
	return (0);
}
