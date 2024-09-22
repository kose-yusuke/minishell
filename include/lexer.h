/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:42:36 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:42:38 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include "token.h"

t_token_type	get_blank_type(char **ps);
t_token_type	get_op_type(char **ps);
t_token_type	get_quoted_type(char **ps, char **q, char **eq);
t_token_type	get_word_or_ionum_type(char **ps, char **q, char **eq);

t_token			*lexer(char *s);

#endif
