/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koseki.yusuke <koseki.yusuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:58:52 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/03/21 23:46:39 by koseki.yusu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "lexer.h"

typedef enum {
    ND_CMD,     // コマンド
    ND_PIPE,    // パイプ
} t_node_kind;

typedef struct s_ast
{
    t_node_kind kind;
    t_ast		*left;
	t_ast		*right;
	t_token		*lst;
}               t_ast;

#endif