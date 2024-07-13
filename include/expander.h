/* expander.h - 変数展開やワイルドカード展開などの文字列展開機能の定義。 */
#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"
# include "ast.h"
# include "env.h"
# include <errno.h>

# define IFS " \t\n" // TODO: move to header file

void	run_expansion(t_cmd *cmd, t_mgr *mgr);
// Util
char	**ft_split(char const *s, char c);


#endif
