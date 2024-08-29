#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
# include <readline/readline.h>

/* signal_hooks.c */
int		heredoc_sigint_hook(void);
int		basic_sigint_hook(void);

/* signal.c */
void	reset_signals(void);
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
