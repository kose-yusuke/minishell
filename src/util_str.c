/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:07:35 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/11 02:00:41 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "xlibc.h"

size_t	ft_strlen(const char *str)
{
	int	a;

	a = 0;
	while (*(str + a) != '\0')
		a++;
	return (a);
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

char	*ft_strdup(char *src)
{
	char	*p;
	long	len;

	if (src == NULL)
		return (NULL);
	len = ft_strlen(src);
	p = (char *)xmalloc(sizeof(char) * (len + 1));
	if (!p)
		return (0);
	ft_strcpy(p, src);
	return (p);
}

char	*ft_strndup(char *src, long len)
{
	char	*p;

	if (src == NULL)
		return (NULL);
	p = (char *)xmalloc(sizeof(char) * (len + 1));
	if (!p)
		return (NULL);
	ft_strncpy(p, src, len);
	p[len] = '\0';
	return (p);
}
