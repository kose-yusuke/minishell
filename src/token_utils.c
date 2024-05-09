/* token_utils.c - トークン関連のユーティリティ関数 */
#include "token.h"

bool	is_word_token(t_token *token)
{
	return (token && token->type == TK_WORD);
}

bool	is_quoted_token(t_token *token)
{
	return (token && (token->type == TK_SQUOTE || token->type == TK_DQUOTE));
}

bool	is_word_or_quoted_token(t_token *token)
{
	return (is_word_token(token) || is_quoted_token(token));
}

bool	is_io_num_token(t_token *token)
{
	return (token && token->type == TK_IO_NUM);
}

bool	is_redir_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->type == TK_REDIR_IN || token->type == TK_REDIR_OUT)
		return (true);
	if (token->type == TK_APPEND || token->type == TK_HEREDOC)
		return (true);
	return (false);
}
