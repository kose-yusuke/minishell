/* executor.h - コマンドの実行とプロセス管理に関する関数の定義。 */
#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "ast.h"
# include "minishell.h"
# include "token.h"

void	run_cmd(t_cmd *cmd, t_mgr *mgr);
void	exec_cmd(t_cmd *cmd, t_mgr *mgr);

/* exec_pipe.c */
void	exec_pipe(t_cmd *cmd, t_mgr *mgr);
/* merge_words.c */
void	merge_words(t_word *word_list);
/* expand_exit_status.c */
void	expand_word_list_for_exit_status(t_word *word_list, int exit_status);

#endif
