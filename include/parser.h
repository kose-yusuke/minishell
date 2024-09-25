/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:47:25 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/25 01:08:52 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ast.h"

/* parser.c - パーサー関連の関数 */
t_cmd	*parser(t_token *token);

/* parser_init.c - パーサーの初期化関数の定義 */
t_arg	*init_arg(t_token *token);
t_redir	*init_redir(t_token_type type, int fd);
t_cmd	*init_execcmd(void);
t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right);

void	append_arg(t_arg **arg_list, t_arg *new_arg);
int		parse_redir(t_redir **redir_list, t_token **token);
#endif
