/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:49:27 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/08/22 02:24:32 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"

int	consume_path(char **rest, char *path, char *s)
{
	size_t	s_len;

	s_len = ft_strlen(s);
	if (strncmp(path, s, s_len) == 0)
	{
		if (*(path + s_len) == '/' || *(path + s_len) == '\0')
		{
			*rest = path + s_len;
			return (1);
		}
	}
	return (0);
}

// /home/userだったら, 二つ目のスラッシュをlast_slash_ptrにして, /userをNULLに変換する.
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
	// pathが絶対パスかoldpwdがない場合
	if (*path == '/' || oldpwd == NULL)
		ft_strlcpy(newpwd, "/", PATH_MAX);
	//相対パスの場合はoldpwdを活用する
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

int	builtin_cd(char **argv, t_mgr *mgr)
{
	char	*pwd;
	char	*path;
	char	*newpwd;

	path = (char *)malloc((sizeof(char)) * PATH_MAX);
	if (!path)
		return (1);
	//今のPWDをOLDPWDに変える
	pwd = getenv("PWD");
	insert(mgr->env_table, "OLDPWD", pwd);
	// pathに新しい作業ディレクトリの値を格納(argv[1]の値で)
	if (set_newpath(&path, argv[1]) == 1)
	{
		free(path);
		perror("no new path");
		return (1);
	}
	// chdirで作業ディレクトリを変更
	// cd ..やcd .は処理可能
	if (chdir(path) < 0)
	{
		free(path);
		perror("path error");
		return (0);
	}
	// PWDを絶対パスで更新
	newpwd = update_pwd(pwd, path);
	if (newpwd)
	{
		// envmapにnewpwdをセット
		insert(mgr->env_table, "PWD", newpwd);
		free(newpwd);
	}
	free(path);
	// statusの値何で返すべきか後で確
	return (1);
}
