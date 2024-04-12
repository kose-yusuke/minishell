/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 02:02:52 by sakitaha          #+#    #+#             */
/*   Updated: 2024/04/12 02:35:29 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <stdbool.h>

typedef enum e_token_kind
{
	TK_WORD,
	TK_OP,
	TK_EOF
}					t_token_kind;

typedef struct s_token
{
	char			*word;
	t_token_kind	kind;
	struct s_token	*next;
}					t_token;

typedef enum
{
	ND_CMD,  // コマンド
	ND_PIPE, // パイプ
}					t_node_kind;

typedef struct s_ast
{
	t_node_kind		kind;
	struct s_ast	*left;
	struct s_ast	*right;
	t_token			*lst;
}					t_ast;

typedef struct s_mgr
{
	t_token			*tok;
	t_ast			*ast;
	int				status;
	bool			syntax_error;
}					t_mgr;

#endif
