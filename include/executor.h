/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:43:49 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:43:51 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/* exec_cmd.c */
t_status	exec_cmd(char **argv, t_mgr *mgr);
/* exec_redir.c */
t_status	exec_redir(t_redir *redir_list, char **argv);

t_status	exec_pipe(t_pipecmd *pcmd, t_mgr *mgr);
t_status	run_cmd(t_cmd *cmd, t_mgr *mgr);

/* exec_pipe_child.c */
pid_t		create_left_process(t_cmd *cmd, int pfd[2], t_mgr *mgr);
pid_t		create_right_process(t_cmd *cmd, int pfd[2], t_mgr *mgr);

#endif
