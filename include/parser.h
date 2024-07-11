/* parser.h - パーサー関連のヘッダーファイル */
#ifndef PARSER_H
# define PARSER_H

# include "ast.h"
# include "minishell.h"
# include <errno.h>

t_cmd	*parser(t_token **token);

/* parser_utils.c - パーサー関連のユーティリティ関数 */
void	advance(t_token **token);
void	skip_blanks(t_token **token);
bool	peek(t_token **token, t_token_type type);
bool	consume(t_token **token, t_token_type type);
t_token	*next_token(t_token **token);

#endif
