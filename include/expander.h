/* expander.h - 変数展開やワイルドカード展開などの文字列展開機能の定義。 */
#ifndef EXPANDER_H
# define EXPANDER_H

# include "ast.h"
# include "env.h"
# include "minishell.h"
# include <errno.h>

# define IFS " \t\n" // TODO: move to header file
void	expand_exit_status(char **word, char **cur_ptr, int exit_status);
void	expand_env(char **word, char **cur_ptr, t_hash_table *env_table);
void	run_expansion(t_cmd *cmd, t_hash_table *env_table);
// Util
char	**ft_split(char const *s, char c);
void	split_word_token(t_word *word);
void	merge_words(t_word *word_list);
void	expand_word_list_for_exit_status(t_word *word_list, int exit_status);
#endif
