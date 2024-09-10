/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_hash_table.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:21:13 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:39:49 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "xlibc.h"

static unsigned long	hash_djb2(unsigned char *str)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	while (*str)
	{
		c = *str++;
		hash = ((hash << 5) + hash) + c;
	}
	return (hash);
}

int	delete(t_hash_table *table, const char *key)
{
	unsigned int	index;
	t_hash_node		*var;
	t_hash_node		*prev;

	index = hash_djb2((unsigned char *)key) % HASH_TABLE_SIZE;
	var = table->table[index];
	prev = NULL;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			if (prev)
				prev->next = var->next;
			else
				table->table[index] = var->next;
			free(var->key);
			free(var->value);
			free(var);
			return (1);
		}
		prev = var;
		var = var->next;
	}
	return (-1);
}

char	*search(t_hash_table *table, const char *key)
{
	unsigned int	index;
	t_hash_node		*var;

	index = hash_djb2((unsigned char *)key) % HASH_TABLE_SIZE;
	var = table->table[index];
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			return (var->value);
		}
		var = var->next;
	}
	return (NULL);
}

char	*insert(t_hash_table *table, char *key, char *value)
{
	unsigned int	index;
	t_hash_node		*new_var;

	if (!key)
		return (NULL);
	delete (table, key);
	index = hash_djb2((unsigned char *)key) % HASH_TABLE_SIZE;
	new_var = xmalloc(sizeof(t_hash_node));
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = table->table[index];
	table->table[index] = new_var;
	return (table->table[index]->value);
}

void	free_hash_table(t_hash_table *table)
{
	t_hash_node	*node;
	t_hash_node	*tmp;
	size_t		i;

	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		node = table->table[i];
		while (node)
		{
			tmp = node;
			node = node->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
		}
		i++;
	}
	free(table);
}
