/* parser.h - パーサー関連のヘッダーファイル */
#ifndef PARSER_H
# define PARSER_H

# include "ast.h"

/* parser.c - パーサー関連の関数 */
t_cmd	*parser(t_token *token);

/* parser_init.c - パーサーの初期化関数の定義 */
t_word	*init_word(t_token *token);
t_redir	*init_redir(t_token_type type, int fd);
t_cmd	*init_execcmd(void);
t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right);

/* parser_utils.c - パーサー関連のユーティリティ関数 */
void	advance(t_token **token);
void	skip_blanks(t_token **token);
bool	peek(t_token **token, t_token_type type);
bool	consume(t_token **token, t_token_type type);
t_token	*next_token(t_token **token);

#endif
