/* executor.h - コマンドの実行とプロセス管理に関する関数の定義。 */
#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "ast.h"
# include "token.h"

void	run_cmd(t_cmd *cmd, t_mgr *mgr);
static void	exec_rightcmd(t_pipecmd *pcmd, int pfd[2], t_mgr *mgr);
static void	exec_leftcmd(t_pipecmd *pcmd, int pfd[2], t_mgr *mgr);
static void     exec_pipe(t_cmd *cmd, t_mgr *mgr);
void     exec_cmd(t_cmd *cmd, t_mgr *mgr);

#endif
