/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:07:35 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/16 12:10:20 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (size < 1)
		return (ft_strlen(src));
	while (src[i] != '\0')
		i++;
	while (src[j] != '\0' && j < size - 1)
	{
		dest[j] = src[j];
		j++;
	}
	dest[j] = '\0';
	return (ft_strlen(src));
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	max_copy;
	size_t	destnum;

	if (dest == NULL)
		return (size + ft_strlen(src));
	destnum = ft_strlen(dest);
	i = 0;
	max_copy = size - destnum - 1;
	if (size <= destnum)
		return (size + ft_strlen(src));
	while (i < max_copy)
	{
		if (src[i] == '\0')
			break ;
		dest[destnum + i] = src[i];
		i++;
	}
	dest[i + destnum] = '\0';
	return (destnum + ft_strlen(src));
}