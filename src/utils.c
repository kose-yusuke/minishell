/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:07:35 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/29 01:02:43 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

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

int	ft_isdigit(int c)
{
	while (c > 47 && c < 58)
	{
		return (1);
	}
	return (0);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			return ((char *)s);
		}
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

size_t	ft_strlen(const char *str)
{
	int	a;

	a = 0;
	while (*(str + a) != '\0')
		a++;
	return (a);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
