#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"

void idle_setup_signals(void);
void exec_parent_setup_signals(void);
void exec_child_setup_signals(void);
void heredoc_parent_setup_signals(void);
void heredoc_child_setup_signals(void);

void idle_handle_signal(int sig);
void exec_handle_signal(int sig);
void heredoc_handle_signal(int sig);
void reset_signal(int signum);
void reset_signals(void);


#endif
