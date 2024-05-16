/* ft_hash.h - ハッシュテーブルの実装 */
#ifndef FT_HASH_H
# define FT_HASH_H

# include "minishell.h" // perror_exitのため
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define HASH_TABLE_SIZE 101 // 適当に決めたサイズ

typedef struct s_hash_node
{
	char				*key;
	char				*value;
	struct s_hash_node	*next;
}						t_hash_node;

typedef struct s_hash_table
{
	t_hash_node			*table[HASH_TABLE_SIZE];
}						t_hash_table;

t_hash_table			*create_table(void);
void					insert(t_hash_table *table, const char *key,
							const char *value);
char					*search(t_hash_table *table, const char *key);
void					free_hash_table(t_hash_table *table);

#endif /* FT_HASH_H */
