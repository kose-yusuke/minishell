/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:49:27 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/24 03:54:25 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

void	append_path(char *dst, char **rest, char *src)
{
	size_t	s_len;

	s_len = 0;
	while (src[s_len] && src[s_len] != '/')
		s_len++;
	if (ft_strlen(dst) + s_len + 1 >= PATH_MAX)
		perror("Destination buffer is too small");
	if (dst[ft_strlen(dst) - 1] != '/' && ft_strlcat(dst, "/",
			PATH_MAX) >= PATH_MAX)
		perror("Destination buffer is too small");
	if (ft_strlcat(dst, src, PATH_MAX) >= PATH_MAX)
		perror("Destination buffer is too small");
	*rest = src + s_len;
}

static char	*update_pwd(char *oldpwd, char *path)
{
	char	*newpwd;

	newpwd = (char *)malloc(PATH_MAX);
	if (!newpwd)
	{
		perror("malloc");
		return (NULL);
	}
	if (*path == '/' || oldpwd == NULL)
		ft_strlcpy(newpwd, "/", PATH_MAX);
	else
		ft_strlcpy(newpwd, oldpwd, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (consume_path(&path, path, ".") == 1)
			;
		else if (consume_path(&path, path, "..") == 1)
			delete_last_path(newpwd);
		else
			append_path(newpwd, &path, path);
	}
	return (newpwd);
}

// homeかargのpathを取得
int	set_newpath(char **path, char *arg)
{
	char	*home;

	if (arg == NULL)
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			perror("cd: HOME not set\n");
			return (1);
		}
		strlcpy(*path, home, PATH_MAX);
	}
	else
		strlcpy(*path, arg, PATH_MAX);
	return (0);
}

static int	handle_cd_error(char *path, const char *error_msg)
{
	free(path);
	perror(error_msg);
	return (1);
}

int	builtin_cd(char **argv, t_mgr *mgr)
{
	char	*pwd;
	char	*path;
	char	*newpwd;

	path = (char *)malloc((sizeof(char)) * PATH_MAX);
	if (!path)
		return (1);
	pwd = getenv("PWD");
	append_env(&(mgr->env_list), "OLDPWD", pwd);
	if (set_newpath(&path, argv[1]) == 1)
		return (handle_cd_error(path, "no new path"));
	if (chdir(path) < 0)
		return (handle_cd_error(path, "path error"));
	newpwd = update_pwd(pwd, path);
	if (newpwd)
		append_env(&(mgr->env_list), "PWD", newpwd);
	else
	{
		free(path);
		return (1);
	}
	free(newpwd);
	free(path);
	return (0);
}
