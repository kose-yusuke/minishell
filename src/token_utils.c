/* token.c */
#include "token.h"

bool	is_word(t_token *token)
{
	return (token && token->type == TK_WORD);
}

bool	is_quoted(t_token *token)
{
	return (token && (token->type == TK_SQUOTE || token->type == TK_DQUOTE));
}

bool	is_word_or_quoted(t_token *token)
{
	return (is_word(token) || is_quoted(token));
}

bool	is_io_num(t_token *token)
{
	return (token && token->type == TK_IO_NUM);
}
