/* readline 関数のラッパー、履歴の初期化と更新、入力の処理と解析など */
#ifndef READLINE_H
# define READLINE_H

# include "expander.h"
# include "heredoc.h"
# include "lexer.h"
# include "parser.h"
# include "token.h"

void	ft_readline(t_mgr *mgr);
void	interpret(char *line, t_mgr *mgr);
void	reset_resources(t_mgr *mgr);

#endif
