/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/10/01 02:25:11 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "error.h"
#include "xlibc.h"

void	display_sorted_env(t_env_node *env_list)
{
	t_env_node	*sorted_env_head;
	t_env_node	*sorted_env;

	sorted_env_head = create_sorted_env(env_list);
	sorted_env = sorted_env_head;
	while (sorted_env)
	{
		printf("declare -x %s", sorted_env->key);
		if (sorted_env->value)
			printf("=\"%s\"\n", sorted_env->value);
		else
			printf("\n");
		sorted_env = sorted_env->next;
	}
	free_env_list(sorted_env_head);
}

int	set_key_value(char *string, t_mgr *mgr)
{
	char	*key;
	char	*value;
	char	*key_end;
	int		ret;

	key = ft_strdup(string);
	if (!key)
		return (-1);
	value = NULL;
	key_end = ft_strchr(key, '=');
	if (key_end)
	{
		*key_end = '\0';
		value = key_end + 1;
	}
	ret = set_env(&mgr->env_list, key, value);
	free(key);
	return (ret);
}

char	*prepare_str_to_print(char *key)
{
	char	*key_to_print;
	size_t	len;

	len = ft_strlen(key);
	key_to_print = xmalloc(len + 3);
	ft_bzero(key_to_print, len + 3);
	key_to_print[0] = '`';
	ft_strlcat(key_to_print, key, len + 3);
	key_to_print[len + 1] = '\'';
	return (key_to_print);
}

int	builtin_export(char **argv, t_mgr *mgr)
{
	int		i;
	int		status;
	char	*str;

	if (argv[1] == NULL)
	{
		display_sorted_env(mgr->env_list);
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (set_key_value(argv[i], mgr) < 0)
		{
			str = prepare_str_to_print(argv[i]);
			report_error("export", str, "not a valid identifier");
			free(str);
			status = 1;
		}
		i++;
	}
	return (status);
}
