/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_get_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:51:47 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/28 19:43:00 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"
#include "xlibc.h"

// static bool is_valid_key_char(char c)
// {
// 	return (ft_isalnum(c) || c == '_');
// }

static char	*extract_env_key(char *env_head, char **suffix)
{
	char	*env_key;
	size_t	len;

	*suffix = env_head;
	while (**suffix && !ft_strchr(IFS_EXTENDED, **suffix))
		(*suffix)++;
	len = *suffix - env_head;
	env_key = xmalloc(len + 1);
	ft_strncpy(env_key, env_head, len);
	env_key[len] = '\0';
	return (env_key);
}

static char	*expand_exit_status(t_status exit_status)
{
	char	*expanded_value;

	expanded_value = ft_itoa(exit_status);
	if (!expanded_value)
	{
		memory_error_and_exit("ft_itoa");
		return (NULL);
	}
	return (expanded_value);
}

static char	*expand_env(char *dollar_ptr, char **suffix, t_env_node *env_list)
{
	char	*env_key;
	char	*expanded_value;

	env_key = extract_env_key(dollar_ptr + 1, suffix);
	expanded_value = get_env(env_list, env_key);
	free(env_key);
	return (expanded_value);
}

char	*get_expanded_value(char *dollar_ptr, char **suffix, t_mgr *mgr)
{
	char	*expanded_value;

	if (*(dollar_ptr + 1) == '?')
	{
		expanded_value = expand_exit_status(mgr->exit_status);
		*suffix = dollar_ptr + 2;
		return (expanded_value);
	}
	expanded_value = expand_env(dollar_ptr, suffix, mgr->env_list);
	if (!expanded_value)
	{
		ft_strcpy(dollar_ptr, *suffix);
		return (NULL);
	}
	return (expanded_value);
}
