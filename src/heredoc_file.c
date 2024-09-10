/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:52:23 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 01:52:35 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "xlibc.h"

static char	*generate_tmp_file_name(int index)
{
	char	*index_str;
	char	*tmp_file_name;
	int		len;

	index_str = ft_itoa(index);
	if (!index_str)
	{
		memory_error_and_exit("ft_itoa");
	}
	len = ft_strlen(FILE_TEMPLATE) + ft_strlen(index_str);
	tmp_file_name = xmalloc(len + 1);
	ft_strcpy(tmp_file_name, FILE_TEMPLATE);
	ft_strlcat(tmp_file_name, index_str, len + 1);
	free(index_str);
	return (tmp_file_name);
}

static char	*create_unique_tmp_file_name(void)
{
	char	*tmp_file_name;
	int		index;

	index = 0;
	while (index < HEREDOC_MAX_FILES)
	{
		tmp_file_name = generate_tmp_file_name(index);
		if (access(tmp_file_name, F_OK) == -1)
		{
			return (tmp_file_name);
		}
		free(tmp_file_name);
		index++;
	}
	report_error("heredoc", NULL, "too many temporary files created");
	return (NULL);
}

int	prepare_tmp_file(char **file_name)
{
	int	fd;

	*file_name = create_unique_tmp_file_name();
	if (!*file_name)
		return (-1);
	fd = open(*file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(*file_name);
		sys_error("heredoc", "open");
		return (-1);
	}
	return (fd);
}

void	delete_tmp_files(void)
{
	int		index;
	char	*file_name;

	index = 0;
	while (1)
	{
		file_name = generate_tmp_file_name(index);
		if (access(file_name, F_OK) == -1)
		{
			free(file_name);
			break ;
		}
		if (unlink(file_name) == -1)
		{
			sys_error("heredoc", "unlink");
			free(file_name);
			break ;
		}
		free(file_name);
		index++;
	}
}
