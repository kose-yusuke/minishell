/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 22:51:59 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/04/12 21:02:01 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "parser.h"

t_ast	*parse_token(t_mgr *mgr)
{
	(void)mgr;
    // ND_PIPEなら新しいnode作って右につける
	return (0);
}
