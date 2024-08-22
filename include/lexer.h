/* lexer.h - 字句解析に関する関数の定義。 */
#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include "token.h"

t_token	*lexer(char *s);

#endif
