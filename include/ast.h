/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:44:06 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 21:43:23 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "token.h"

typedef struct s_arg
{
	struct s_token		*token;
	struct s_arg		*next;
}						t_arg;

typedef struct s_redir
{
	enum e_token_type	redir_type;
	int					fd;
	struct s_arg		*arg_list;
	struct s_redir		*next;
}						t_redir;

typedef enum e_cmd_type
{
	EXEC,
	PIPE
}						t_cmd_type;

typedef struct s_cmd
{
	enum e_cmd_type		type;
}						t_cmd;

typedef struct s_execcmd
{
	enum e_cmd_type		type;
	struct s_arg		*arg_list;
	struct s_redir		*redir_list;
}						t_execcmd;

typedef struct s_pipecmd
{
	enum e_cmd_type		type;
	struct s_cmd		*left;
	struct s_cmd		*right;
}						t_pipecmd;

#endif
