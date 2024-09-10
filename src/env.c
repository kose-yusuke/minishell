/* env.c */
#include "env.h"
#include "minishell.h"
#include "utils.h"
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

int delete (t_hash_table *table, const char *key)
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

char	*insert(t_hash_table *table, char *key, char *value)
{
	unsigned int	index;
	t_hash_node		*new_var;

	if (!key)
		return (NULL);
	delete (table, key); // 既存のkeyがあれば削除
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

t_hash_table	*create_hash_table(void)
{
	t_hash_table	*table;

	table = malloc(sizeof(t_hash_table));
	if (!table)
		return (NULL);
	ft_bzero(table, sizeof(t_hash_table));
	table->search = search;
	table->delete = delete;
	table->insert = insert;
	table->free = free_hash_table;
	return (table);
}

t_hash_table	*create_env_table(void)
{
	extern char		**environ;
	t_hash_table	*env_table;
	char			*env;
	char			*delimiter;
	size_t			i;

	env_table = create_hash_table();
	if (!env_table)
	{
		return (NULL);
	}
	i = 0;
	while (environ[i])
	{
		env = strdup(environ[i]);
		if (!env)
		{
			free_hash_table(env_table);
			return (NULL);
		}
		delimiter = strchr(env, '=');
		if (delimiter) // if(delimiter && *delimiter) ？
		{
			*delimiter = '\0';
			env_table->insert(env_table, env, delimiter + 1);
		}
		free(env);
		i++;
	}
	return (env_table);
}
