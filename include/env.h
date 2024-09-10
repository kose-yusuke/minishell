/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:48:43 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:50:40 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# define HASH_TABLE_SIZE 101

typedef struct s_hash_node
{
	char				*key;
	char				*value;
	struct s_hash_node	*next;
}						t_hash_node;

typedef struct s_hash_table
{
	t_hash_node			*table[HASH_TABLE_SIZE];
	char				*(*search)(struct s_hash_table *table, const char *key);
	int					(*delete)(struct s_hash_table *table, const char *key);
	char				*(*insert)(struct s_hash_table *table, char *key,
			char *value);
	void				(*free)(struct s_hash_table *table);
}						t_hash_table;

t_hash_table			*create_hash_table(void);

char					*insert(t_hash_table *table, char *key, char *value);
char					*search(t_hash_table *table, const char *key);
int						delete(t_hash_table *table, const char *key);
void					free_hash_table(t_hash_table *table);

t_hash_table			*create_env_table(void);

#endif
