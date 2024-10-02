/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:06:43 by sakitaha          #+#    #+#             */
/*   Updated: 2024/10/02 16:18:30 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strtol.h"

int	ft_isspace_custom(int c)
{
	return (c == ' ' || c == '\t');
}

void	skip_space(const char **str)
{
	while (ft_isspace_custom(**str))
	{
		(*str)++;
	}
}

bool	check_sign(const char **str)
{
	bool	is_negative;

	is_negative = (**str == '-');
	if (**str == '-' || **str == '+')
	{
		(*str)++;
	}
	return (is_negative);
}

bool	is_valid_digit(char c, int base)
{
	if (base == 0)
	{
		return ('0' <= c && c <= '9');
	}
	if (base <= 10)
	{
		return ('0' <= c && c < '0' + base);
	}
	return (('0' <= c && c <= '9') || ('A' <= c && c < 'A' + base - 10)
		|| ('a' <= c && c < 'a' + base - 10));
}
