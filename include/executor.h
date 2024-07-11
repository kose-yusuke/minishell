/* executor.h - コマンドの実行とプロセス管理に関する関数の定義。 */
#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "ast.h"
# include "minishell.h"
# include "token.h"

// void	validate_access(const char *path, const char *filename);
// int		exec(char *argv[]);
void	run_cmd(t_cmd *cmd);
// static void	exec_leftcmd(t_pipecmd *pcmd, int pfd[2]);
// static void	exec_rightcmd(t_pipecmd *pcmd, int pfd[2]);
// static void	exec_pipe(t_cmd *cmd);
// static void	exec_cmd(t_cmd *cmd);
void	exec_redir(t_cmd *cmd);


#endif
