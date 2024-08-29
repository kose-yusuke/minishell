/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:48:16 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/22 13:16:41 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
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

int	builtin_exit(char **argv)
{
	char	*arg;
	int		status;

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
		errno = 0;
		status = ft_atoi(arg);
		if (errno == 0)
			exit(status);
	}
	perror("exit: numeric argument required"); // 数字でない引数の場合のエラーメッセージ
	exit(255);                                 // エラーの場合は255で終了
}
