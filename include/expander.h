/* expander.h - 変数展開やワイルドカード展開などの文字列展開機能の定義。 */
#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"

# define IFS " \t\n\""
# define IFS_EXTENDED " \t\n\"\'|&;()<>$"

/* exp_get_value.c */
char	*get_expanded_value(char *dollar_ptr, char **suffix, t_mgr *mgr);

/* exp_word_str.c */
void	expand_word_str(char **str, t_mgr *mgr);

/* expander.c */
void	run_expansion(t_cmd *cmd, t_mgr *mgr);

void	split_word_token(t_word *word);
void	merge_words(t_word *word_list);

#endif
