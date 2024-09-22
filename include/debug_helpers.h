/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_helpers.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:46:56 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:46:56 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HELPERS_H
# define DEBUG_HELPERS_H

# include "ast.h"
# include "utils.h"

/* debug_helpers.c */
void	print_status(int status, char *location);
void	print_tokens(t_token *token);
void	print_word_list(t_word *word_list);
void	print_redir_list(t_redir *redir_list);
void	print_cmd(t_cmd *cmd);

#endif // DEBUG_HELPERS_H
