/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 23:27:42 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/24 21:48:27 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* builtins.h - シェルのビルトインコマンド (cd, echo, exit など) のプロトタイプと関連データの定義。 */
#ifndef BUILTIN_CMD_H
# define BUILTIN_CMD_H

# include "minishell.h"
# include <limits.h>

/* builtin.c */
bool	is_builtin(char *cmd_name);
int		exec_builtin(char **argv, t_mgr *mgr);
char	**convert_list_to_array(t_arg *arg_list);
void	print_argv(char **argv);
int		consume_path(char **rest, char *path, char *s);
void	delete_last_path(char *newpwd);

/* builtin_exit.c */
int		builtin_exit(char **argv);
/* builtin_export.c */
int		builtin_export(char **argv, t_mgr *mgr);
char	*prepare_str_to_print(char *key);
/* builtin_unset.c */
int		builtin_unset(char **argv, t_mgr *mgr);
/* builtin_pwd.c */
int		builtin_pwd(char **argv);
/* builtin_env.c */
int		builtin_env(char **argv, t_mgr *mgr, int var);
/* builtin_echo.c */
int		builtin_echo(char **argv);
/* builtin_cd.c */
int		builtin_cd(char **argv, t_mgr *mgr);

#endif
