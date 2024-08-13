/* debug_helpers.h - debug用の関数を定義するヘッダファイル */
#ifndef DEBUG_HELPERS_H
# define DEBUG_HELPERS_H

# include "minishell.h"

void	print_tokens(t_token *token);
void	print_word_list(t_word *word_list);
void	print_redir_list(t_redir *redir_list);
void	print_cmd(t_cmd *cmd);

#endif // DEBUG_HELPERS_H