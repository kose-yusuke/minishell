/* ft_hash.h - ハッシュテーブルの実装 */
#ifndef FT_HASH_H
# define FT_HASH_H

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
	char				*(*insert)(struct s_hash_table *table, char *key,
						char *value);
	char				*(*search)(struct s_hash_table *table, const char *key);
	int					(*delete)(struct s_hash_table *table, const char *key);
	void				(*free)(struct s_hash_table *table);
}						t_hash_table;

# include "env.h" // perror_exitのため

t_hash_table			*create_hash_table(void);
void	free_hash_table(t_hash_table *table);
t_hash_table	*create_hash_table(void);
char	*insert(t_hash_table *table, char *key, char *value);
char	*search(t_hash_table *table, const char *key);
int delete(t_hash_table *table, const char *key);

#endif /* FT_HASH_H */
