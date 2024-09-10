/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 22:04:16 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/10 13:16:31 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

int	consume_path(char **rest, char *path, char *s)
{
	size_t	s_len;

	s_len = ft_strlen(s);
	if (ft_strncmp(path, s, s_len) == 0)
	{
		if (*(path + s_len) == '/' || *(path + s_len) == '\0')
		{
			*rest = path + s_len;
			return (1);
		}
	}
	return (0);
}

void	delete_last_path(char *newpwd)
{
	char	*origin;
	char	*last_slash_ptr;

	origin = newpwd;
	last_slash_ptr = NULL;
	while (*newpwd)
	{
		if (*newpwd == '/')
			last_slash_ptr = newpwd;
		newpwd++;
	}
	if (origin != last_slash_ptr)
		*last_slash_ptr = '\0';
}
