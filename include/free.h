/* free.h - メモリ解放関連のヘッダーファイル */
#ifndef FREE_H
# define FREE_H

# include "minishell.h"

/* free_ast.c */
void	free_word(t_word *word);
void	free_redir(t_redir *redir);
void	free_cmd(t_cmd *cmd);

/* free.c */
void	free_tokens(t_token *token);
void	free_mgr_resources(t_mgr *mgr);
void	free_argv(char **argv);

#endif
