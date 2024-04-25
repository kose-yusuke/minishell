/* lexer.h - 入力をトークンに分割する字句解析器の定義。 */
#ifndef LEXER_H
# define LEXER_H
# include "struct.h"

t_token	*tokenize(char *s);

#endif
