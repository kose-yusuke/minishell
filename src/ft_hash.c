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

void	insert(t_hash_table *table, const char *key, const char *value)
{
	unsigned int	index;
	t_hash_node		*new_var;

	index = hash_djb2((unsigned char *)key) % HASH_TABLE_SIZE;
	new_var = malloc(sizeof(t_hash_node));
	if (!new_var)
	{
		free_hash_table(table);
		perror_exit("malloc");
	}
	new_var->key = strdup(key);
	new_var->value = strdup(value);
	if (!new_var->key || !new_var->value)
	{
		free(new_var->key);
		free(new_var);
		free_hash_table(table);
		perror_exit("strdup");
	}
	new_var->next = table->table[index];
	table->table[index] = new_var;
}

char	*search(t_hash_table *table, const char *key)
{
	unsigned int	index;
	t_hash_node		*var;

	index = hash_djb2((unsigned char *)key) % HASH_TABLE_SIZE;
	var = table->table[index];
	while (var)
	{
		if (strcmp(var->key, key) == 0)
		{
			return (var->value);
		}
		var = var->next;
	}
	return (NULL);
}

void	free_hash_index(t_hash_node *node)
{
	t_hash_node	*tmp;

	while (node)
	{
		tmp = node;
		node = node->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	free_hash_table(t_hash_table *table)
{
	size_t	i;

	i = 0;
	while (i < HASH_TABLE_SIZE)
	{
		free_hash_index(table->table[i]);
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
	return (table);
}



