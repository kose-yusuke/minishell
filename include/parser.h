/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:47:25 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:47:27 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ast.h"

/* parser.c - パーサー関連の関数 */
t_cmd	*parser(t_token *token);

/* parser_init.c - パーサーの初期化関数の定義 */
t_word	*init_word(t_token *token);
t_redir	*init_redir(t_token_type type, int fd);
t_cmd	*init_execcmd(void);
t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right);

void	append_word(t_word **word_list, t_word *new_word);
int		parse_redir(t_redir **redir_list, t_token **token);
#endif
