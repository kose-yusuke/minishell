/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:48:16 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/10/02 17:55:12 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "error.h"
#include "ft_strtol.h"
#include <errno.h>

static int	check_overflow(long long num, int onedigit, int type)
{
	if (type == 1 && ((num > LLONG_MAX / 10) || (num == LLONG_MAX / 10
				&& onedigit > 7)))
		return (-1);
	if (type == -1 && ((-1 * num < LLONG_MIN / 10) || (-1 * num == LLONG_MIN
				/ 10 && onedigit > 8)))
		return (0);
	return (1);
}

long	ft_atoi(const char *str)
{
	int			type;
	long long	num;
	int			overflow;

	type = 1;
	num = 0;
	while (*str != '\0' && (*str == ' ' || *str == '\f' || *str == '\n'
			|| *str == '\r' || *str == '\t' || *str == '\v'))
		str++;
	if (*str != '\0' && (*str == '+' || *str == '-'))
	{
		if (*str == '-')
			type *= -1;
		str++;
	}
	while (*str != '\0' && *str >= '0' && *str <= '9')
	{
		overflow = check_overflow(num, *str - '0', type);
		if (overflow == -1 || overflow == 0)
			return (overflow);
		num = num * 10 + (*str - '0');
		str++;
	}
	return (type * num);
}

static bool	is_valid_endptr(char *endptr)
{
	if (!endptr)
		return (false);
	while (*endptr)
	{
		if (!ft_isspace_custom(*endptr))
			return (false);
		endptr++;
	}
	return (true);
}

static long	parse_exit_arg(char **argv)
{
	long	status_long;
	char	*endptr;

	endptr = NULL;
	errno = 0;
	status_long = ft_strtol(argv[1], &endptr, 10);
	if (errno != 0 || !is_valid_endptr(endptr))
	{
		report_error("exit", argv[1], "numeric argument required");
		return (255);
	}
	if (argv[2])
	{
		report_error("exit", 0, "too many arguments");
		return (1);
	}
	return (status_long);
}

int	builtin_exit(char **argv)
{
	long	status_long;

	if (argv[1] == NULL)
		exit(0);
	status_long = parse_exit_arg(argv);
	if (status_long == 1 && argv[2])
	{
		return (1);
	}
	exit((int)status_long & 0xff);
}
