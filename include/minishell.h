/*  minishell.h - minishellのヘッダファイル*/
#ifndef MINISHELL_H
# define MINISHELL_H

# include "ast.h"
# include "ft_hash.h"
# include "utils.h"
# include <dirent.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_mgr
{
	int							exit_status;
	t_token						*token;
	t_cmd						*cmd;
	t_hash_table				*env_table;
}								t_mgr;

extern volatile sig_atomic_t	g_signal_status;

#endif
