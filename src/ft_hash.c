/* ft_hash.c - ハッシュテーブルの実装 */
#include "ft_hash.h"

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

static void	insert(t_hash_table *table, const char *key, const char *value)
{
	unsigned int	index;
	t_hash_node		*new_var;

	index = hash_djb2((unsigned char *)key) % HASH_TABLE_SIZE;
	new_var = malloc(sizeof(t_hash_node));
	if (!new_var)
	{
		table->free(table);
		perror_exit("malloc");
	}
	new_var->key = strdup(key);
	new_var->value = strdup(value);
	if (!new_var->key || !new_var->value)
	{
		free(new_var->key);
		free(new_var);
		table->free(table);
		perror_exit("strdup");
	}
	new_var->next = table->table[index];
	table->table[index] = new_var;
}

static char	*search(t_hash_table *table, const char *key)
{
	unsigned int	index;
	t_hash_node		*var;

	index = hash_djb2((unsigned char *)key) % HASH_TABLE_SIZE;
	var = table->table[index];
	while (var)
	{
		// 比較の仕方は大丈夫か？
		if (strcmp(var->key, key) == 0)
		{
			return (var->value);
		}
		var = var->next;
	}
	return (NULL);
}

static void	free_hash_table(t_hash_table *table)
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
	memset(table, 0, sizeof(t_hash_table));
	table->insert = insert;
	table->search = search;
	table->free = free_hash_table;
	return (table);
}
