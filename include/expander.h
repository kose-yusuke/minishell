/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:43:24 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/30 14:11:43 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"

# define IFS " \t\n\""

/* exp_get_value.c */
char	*get_expanded_value(char *dollar_ptr, char **suffix, t_mgr *mgr);

/* exp_word_str.c */
void	expand_word_str(char **str, t_mgr *mgr);

/* expander.c */
void	run_expansion(t_cmd *cmd, t_mgr *mgr);

t_arg	*new_arg_node(t_token_type type, char *str);
void	split_word_token(t_arg *arg);
void	merge_arg_list(t_arg *arg_list);
void	expand_arg_list(t_arg *arg_list, t_mgr *mgr);

#endif
