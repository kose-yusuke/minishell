/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:45:12 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:45:14 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
# include <readline/readline.h>

/* signal_hooks.c */
void	handle_sigint(int sig);
int		heredoc_sigint_hook(void);
int		basic_sigint_hook(void);

/* signal.c */
void	ignore_signal(int signum);
void	restore_signal(int signum);
void	restore_signals(void);
void	init_signal(void);

// void	idle_setup_signals(void);
// void	exec_parent_setup_signals(void);
// void	exec_child_setup_signals(void);
// void	heredoc_parent_setup_signals(void);
// void	heredoc_child_setup_signals(void);

// void	init_signal_handler(void);
// void	idle_handle_signal(int sig);
// void	exec_handle_signal(int sig);
// void	heredoc_handle_signal(int sig);

#endif
