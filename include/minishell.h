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
	SC_SUCCESS = 0,        // 正常終了
	SC_FAILURE = 1,        // 一般的なエラー
	SC_BADUSAGE = 2,       // ビルトインコマンドの誤用
	SC_MISCERROR = 2,      // その他のエラー
	SC_RETRYFAIL = 124,    // 再試行の失敗
	SC_WEXPCOMSUB = 125,   // コマンド置換に失敗
	SC_BINARY_FILE = 126,  // 実行できないバイナリファイル
	SC_NOEXEC = 126,       // 実行できないコマンド
	SC_NOINPUT = 126,      // 入力がない
	SC_NOTFOUND = 127,     // コマンドが見つからない
	SC_FATAL_ERROR = 255,  // 致命的なエラー
	SC_SHERRBASE = 256,    // すべての特別なエラー値はこれより大きい
	SC_BADSYNTAX = 257,    // シェル構文エラー
	SC_USAGE = 258,        // 使用方法の誤り
	SC_REDIRFAIL = 259,    // リダイレクトに失敗
	SC_BADASSIGN = 260,    // 変数の代入エラー
	SC_EXPFAIL = 261,      // ワード展開に失敗
	SC_DISKFALLBACK = 262, // ディスクコマンドへのフォールバック
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
