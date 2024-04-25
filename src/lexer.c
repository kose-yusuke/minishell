#include "lexer.h"
#include "minishell.h"

static t_token_kind	get_blank_token(char **ps)
{
	static const char	*whitespace = " \t";
	char				*s;

	s = *ps;
	if (*s && *s != '\n' && !strchr(whitespace, *s))
		return (TK_UNDEF_TOKEN);
	if (*s == '\n')
	{
		while (*s && *s == '\n')
			s++;
		*ps = s;
		return (TK_NEWLINE);
	}
	while (*s && strchr(whitespace, *s))
		s++;
	*ps = s;
	return (TK_BLANK);
}

static t_token_kind	get_op_token(char **ps)
{
	static const char			*op[] = {"|", "<<", ">>", "<", ">", NULL};
	static const t_token_kind	tok[] = {TK_PIPE, TK_HEREDOC, TK_REDIR_APPEND,
			TK_REDIR_IN, TK_REDIR_OUT};
	static const size_t			len[] = {1, 2, 2, 1, 1};
	char						*s;
	size_t						i;

	s = *ps;
	i = 0;
	while (op[i])
	{
		if (strncmp(s, op[i], len[i]) == 0)
		{
			s += len[i];
			*ps = s;
			return (tok[i]);
		}
		i++;
	}
	return (TK_UNDEF_TOKEN);
}

static t_token_kind	get_quoted_token(char **ps, char **q, char **eq)
{
	char	*s;
	char	quote_char;

	s = *ps;
	quote_char = *s;
	if (quote_char != '\'' && quote_char != '"')
		return (TK_UNDEF_TOKEN);
	*q = ++s;
	while (*s && *s != quote_char)
		s++;
	if (*s != quote_char)
		return (TK_PARSE_ERROR);
	*eq = s;     // token化の際にヌル終端すべき場所
	*ps = s + 1; // 次のトークンの先頭
	if (quote_char == '\'')
		return (TK_SQUOTED_STR);
	else
		return (TK_DQUOTED_STR);
}

static bool	is_meta_character(char c)
{
	static const char	*metacharacter = "|<> \t\n'\"";

	return (strchr(metacharacter, c) != 0);
}

static t_token_kind	get_word_token(char **ps, char **q, char **eq)
{
	char	*s;

	s = *ps;
	*q = s;
	if (is_meta_character(*s))
		return (TK_UNDEF_TOKEN);
	while (*s && !is_meta_character(*s))
		s++;
	*eq = s; // token化の際にヌル終端すべき位置
	*ps = s; // 次のトークンの先頭
	return (TK_WORD);
}

static t_token_kind	get_token_kind(char **ps, char **q, char **eq)
{
	t_token_kind	kind;

	kind = get_blank_token(ps);
	if (kind != TK_UNDEF_TOKEN)
		return (kind);
	kind = get_op_token(ps);
	if (kind != TK_UNDEF_TOKEN)
		return (kind);
	kind = get_quoted_token(ps, q, eq);
	if (kind != TK_UNDEF_TOKEN)
		return (kind);
	kind = get_word_token(ps, q, eq);
	if (kind != TK_UNDEF_TOKEN)
		return (kind);
	return (TK_PARSE_ERROR);
}

/*
TODO: 失敗時に確保済みのリソースを適切に解放する処理を追加する
 */
static t_token	*new_token(t_token_kind kind, char **q, char **eq)
{
	t_token	*new_tok;

	new_tok = calloc(1, sizeof(t_token));
	if (!new_tok)
		fatal_error("calloc");
	new_tok->kind = kind;
	if (kind == TK_WORD || kind == TK_SQUOTED_STR || kind == TK_DQUOTED_STR)
	{
		if (!q || !eq || !*q)
			error_exit("word string is not found");
		new_tok->word = strndup(*q, *eq - *q);
		if (!new_tok->word)
			fatal_error("strndup");
	}
	return (new_tok);
}

t_token	*tokenize(char *s)
{
	t_token	head_tok;
	t_token	*cur_tok;
	char	*q;
	char	*eq;

	head_tok.next = NULL;
	cur_tok = &head_tok;
	while (*s)
	{
		q = NULL;
		eq = NULL;
		cur_tok->next = new_token(get_token_kind(&s, &q, &eq), &q, &eq);
		if (!cur_tok->next || cur_tok->next->kind == TK_PARSE_ERROR)
			assert_error("Unexpected Token");
		cur_tok = cur_tok->next;
	}
	cur_tok->next = new_token(TK_EOF, NULL, NULL);
	return (head_tok.next);
}
