/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:46:36 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/23 22:02:06 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ast.h"
# include "env.h"
# include "utils.h"
# include <dirent.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_status
{
	SC_SUCCESS = 0,
	SC_FAILURE = 1,
	SC_BADUSAGE = 2,
	SC_MISCERROR = 2,
	SC_RETRYFAIL = 124,
	SC_WEXPCOMSUB = 125,
	SC_BINARY_FILE = 126,
	SC_NOEXEC = 126,
	SC_NOINPUT = 126,
	SC_NOTFOUND = 127,
	SC_FATAL_ERROR = 255,
	SC_SHERRBASE = 256,
	SC_BADSYNTAX = 257,
	SC_USAGE = 258,
	SC_REDIRFAIL = 259,
	SC_BADASSIGN = 260,
	SC_EXPFAIL = 261,
	SC_DISKFALLBACK = 262
}								t_status;

typedef struct s_mgr
{
	t_status					exit_status;
	t_token						*token;
	t_cmd						*cmd;
	t_env_node					*env_list;
}								t_mgr;

extern volatile sig_atomic_t	g_status;

#endif

// bashの終了ステータス
// /* Values that can be returned by execute_command (). */
// #define EXECUTION_FAILURE 1
// #define EXECUTION_SUCCESS 0

// /* Usage messages by builtins result in a return status of 2. */
// #define EX_BADUSAGE	2

// #define EX_MISCERROR	2

// /* Special exit statuses used by the shell, internally and externally. */
// #define EX_RETRYFAIL	124
// #define EX_WEXPCOMSUB	125
// #define EX_BINARY_FILE	126
// #define EX_NOEXEC	126
// #define EX_NOINPUT	126
// #define EX_NOTFOUND	127

// #define EX_SHERRBASE	256	/* all special error values are > this. */

// #define EX_BADSYNTAX	257	/* shell syntax error */
// #define EX_USAGE	258	/* syntax error in usage */
// #define EX_REDIRFAIL	259	/* redirection failed */
// #define EX_BADASSIGN	260	/* variable assignment error */
// #define EX_EXPFAIL	261	/* word expansion failed */
// #define EX_DISKFALLBACK	262	/* fall back to disk command from
