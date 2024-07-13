/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 18:21:45 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/07/13 18:34:34 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*ans;
	int		i;
	size_t	mem;

	i = 0;
	mem = count * size;
	if (size != 0 && count >= SIZE_MAX / size)
		return (NULL);
	ans = (char *)malloc(sizeof(char) * (mem));
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

static int	ft_split_number(char *s, char c)
{
	int	cnt;
	int	i;

	cnt = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] != c)
			cnt++;
		while (s[i] != c && s[i] != '\0')
			i++;
		if (s[i] == '\0')
			return (cnt);
	}
	return (cnt);
}

static int	ft_strlen_alpha(char *str, char charset)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != charset)
		i++;
	return (i);
}

static char	*each_phrase(char *str, char charset)
{
	int		size2;
	int		i;
	char	*ans;

	if (str == NULL)
		return (NULL);
	i = 0;
	size2 = ft_strlen_alpha(str, charset);
	ans = (char *)malloc(sizeof(char) * (size2 + 1));
	if (!ans)
		return (NULL);
	while (str[i] != charset && str[i] != '\0')
	{
		ans[i] = str[i];
		i++;
	}
	ans[i] = '\0';
	return (ans);
}

static char	**ft_all_free(char **output)
{
	size_t	i;

	i = 0;
	while (output[i] != NULL)
	{
		free(output[i]);
		i++;
	}
	free(output);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**output;
	int		i;

	if (s == NULL)
		return (NULL);
	i = 0;
	output = ft_calloc(ft_split_number((char *)s, c) + 1, sizeof(char *));
	if (!output)
		return (NULL);
	while (*s != '\0')
	{
		while (*s != '\0' && *s == c)
			s++;
		if (*s != '\0')
		{
			output[i] = each_phrase((char *)s, c);
			if (output[i] == NULL)
				return (ft_all_free(output));
			i++;
		}
		while (*s != '\0' && *s != c)
			s++;
	}
	return (output);
}