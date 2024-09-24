/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:48:43 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 17:25:27 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

typedef struct s_env_node
{
	char				*key;
	char				*value;
	struct s_env_node	*next;
}						t_env_node;

t_env_node				*create_env_node(char *key, char *value);
void					insert_to_sorted_list(t_env_node **sorted_head,
							t_env_node *node_to_add);
t_env_node				*create_sorted_env(t_env_node *env_head);
void					free_env_list(t_env_node *env_head);
int						unset_env(t_env_node **env_head, char *key);
char					*get_env(t_env_node *env_head, char *key);
int						set_env(t_env_node **env_head, char *key, char *value);
void					add_var_to_env_list(t_env_node **env_head,
							char *env_var);
t_env_node				*create_env_list(void);

#endif
