/* readline 関数のラッパー、履歴の初期化と更新、入力の処理と解析など */
#ifndef READLINE_H
# define READLINE_H

# include "lexer.h"
# include "parser.h"
# include "token.h"
# include "expander.h"
# include <readline/history.h>
# include <readline/readline.h>

int	ft_readline(t_mgr *mgr);

#endif
