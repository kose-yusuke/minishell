/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:13:13 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 19:55:48 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "utils.h"
#include "xlibc.h"

void	free_env_list(t_env_node *env_head)
{
	t_env_node	*node;
	t_env_node	*tmp;

	if (!env_head)
		return ;
	node = env_head;
	while (node)
	{
		tmp = node;
		node = node->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

t_env_node	*create_env_node(char *key, char *value)
{
	t_env_node	*new_node;

	new_node = xmalloc(sizeof(t_env_node));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

void	add_var_to_env_list(t_env_node **env_head, char *env_var)
{
	char	*copied_key;
	char	*copied_value;
	char	*eql_sign;

	copied_key = ft_strdup(env_var);
	if (!copied_key)
		return ;
	copied_value = NULL;
	eql_sign = ft_strchr(copied_key, '=');
	if (eql_sign)
	{
		*eql_sign = '\0';
		copied_value = eql_sign + 1;
	}
	set_env(env_head, copied_key, copied_value);
	free(copied_key);
}

t_env_node	*create_env_list(void)
{
	extern char	**environ;
	t_env_node	*env_head;
	size_t		i;

	if (!environ)
		return (NULL);
	env_head = NULL;
	i = 0;
	while (environ[i])
	{
		add_var_to_env_list(&env_head, environ[i]);
		i++;
	}
	return (env_head);
}
