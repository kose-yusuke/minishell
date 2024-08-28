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

typedef enum e_status
{
	SC_SUCCESS = 0,              // 正常終了
	SC_GENERAL_ERROR = 1,        // 一般的なエラー
	SC_BUILTIN_MISUSE = 2,       // built-inコマンドの誤用
	SC_CMD_NOT_EXECUTABLE = 126, // 実行できないコマンド ex. /: is a directory
	SC_CMD_NOT_FOUND = 127,      // コマンドが見つからない
	// SC_INVALID_EXIT_ARG = 128,   // Invalid argument to exit
	SC_SIGINT = 130,       // Ctrl+C による終了
	SC_SIGQUIT = 131,      // Ctrl+\ による終了
	SC_SYNTAX_ERROR = 258, // 構文エラー
	SC_FATAL_ERROR = 255   // 致命的なエラー
}								t_status;

typedef struct s_mgr
{
	t_status					exit_status;
	t_token						*token;
	t_cmd						*cmd;
	t_hash_table				*env_table;
}								t_mgr;

extern volatile sig_atomic_t	g_status;

#endif
