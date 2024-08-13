/* parser_utils.c - パーサー関連のユーティリティ関数 */
#include "parser.h"

/**
 * トークンストリームを次に進める
 */
void	advance(t_token **token)
{
	if (*token && (*token)->type != TK_EOF)
		*token = (*token)->next;
}

/**
 * 空白（スペース、タブ）をスキップする。
 * これにより、トークン解析時に無視すべき文字を効率的に処理する。
 */
void	skip_blanks(t_token **token)
{
	while (*token && (*token)->type == TK_BLANK)
		advance(token);
}

/**
 * 現在のトークンが期待されるトークンタイプと一致するか見る。blankは消費
 * トークン自体は消費はしない
 */
bool	peek(t_token **token, t_token_type type)
{
	if (!token || !*token)
		return (false);
	skip_blanks(token);
	return (*token && (*token)->type == type);
}

/**
 * 現在のトークンが期待されるトークンタイプの時にはそれを消費し、真を返す。
 * 空白トークンを先にスキップし、それ以外のトークンや、トークン自体がNULLの場合には偽を返す。
 */
bool	consume(t_token **token, t_token_type type)
{
	bool	ret;

	ret = peek(token, type);
	if (ret)
		advance(token);
	return (ret);
}

/**
 * トークンストリームから次の有効なトークンを取得し、それを返す。
 * 空白トークンはスキップされる。ポインタは、次のトークンに進む。
 * 次のトークンが存在しない場合は NULL を返す。
 */
t_token	*next_token(t_token **token)
{
	t_token	*current;

	skip_blanks(token);
	if (!*token)
		return (NULL);
	if ((*token)->type == TK_EOF)
		return (*token);
	current = *token;
	advance(token);
	return (current);
}
