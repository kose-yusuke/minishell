/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:44:56 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 01:48:17 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "xlibc.h"

void	ft_bzero(void *s, size_t n)
{
	char	*p;
	int		i;

	p = (char *)s;
	i = 0;
	while (n > 0)
	{
		p[i] = 0;
		n--;
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	char	*ans;
	int		i;
	size_t	mem;

	i = 0;
	mem = count * size;
	if (size != 0 && count >= SIZE_MAX / size)
		return (NULL);
	ans = (char *)xmalloc(sizeof(char) * (mem));
	if (!ans)
		return (NULL);
	while (mem > 0)
	{
		ans[i] = 0;
		i++;
		mem--;
	}
	return (ans);
}

static size_t	count_digits(long long num)
{
	size_t	len;

	len = 0;
	if (num == 0)
	{
		return (1);
	}
	while (num)
	{
		num /= 10;
		len++;
	}
	return (len);
}

static void	num_to_str(long long num, char *str, size_t len, bool is_negative)
{
	while (len--)
	{
		str[len + is_negative] = '0' + num % 10;
		num /= 10;
	}
	if (is_negative)
	{
		str[0] = '-';
	}
}

char	*ft_itoa(int n)
{
	long long	num;
	size_t		len;
	bool		is_negative;
	char		*str;

	num = (long long)n;
	is_negative = false;
	if (num < 0)
	{
		is_negative = true;
		num = -num;
	}
	len = count_digits(num);
	str = (char *)ft_calloc(len + is_negative + 1, sizeof(char));
	if (!str)
	{
		return (NULL);
	}
	num_to_str(num, str, len, is_negative);
	return (str);
}
