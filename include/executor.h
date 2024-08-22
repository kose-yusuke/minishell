/* executor.h - コマンドの実行とプロセス管理に関する関数の定義。 */
#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

void	run_cmd(t_cmd *cmd, t_mgr *mgr);
int		exec_cmd(t_cmd *cmd, t_mgr *mgr);
void	exec_pipe(t_cmd *cmd, t_mgr *mgr);

int		exec_redir(t_redir *redir_list, t_mgr *mgr);

#endif
