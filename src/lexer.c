/* lexer.c - 入力をトークンに分割する字句解析器の実装。 */
#include "lexer.h"

static t_token_type	get_blank_token(char **ps)
{
	char	token_type;

	if (!**ps || !strchr(" \t\n", **ps))
	{
		return (TK_UNDEF_TOKEN);
	}
	token_type = **ps;
	while (**ps && **ps == token_type)
	{
		(*ps)++;
	}
	return (token_type);
}

static t_token_type	get_op_token(char **ps)
{
	static const char			*op[] = {"|", "<<", ">>", "<", ">", NULL};
	static const t_token_type	tok[] = {TK_PIPE, TK_HEREDOC, TK_REDIR_APPEND,
			TK_REDIR_IN, TK_REDIR_OUT, TK_UNDEF_TOKEN};
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

static t_token_type	get_quoted_token(char **ps, char **q, char **eq)
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

static t_token_type	get_word_token(char **ps, char **q, char **eq)
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

static t_token_type	get_token_type(char **ps, char **q, char **eq)
{
	t_token_type	type;

	type = get_blank_token(ps);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	type = get_op_token(ps);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	type = get_quoted_token(ps, q, eq);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	type = get_word_token(ps, q, eq);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	return (TK_PARSE_ERROR);
}

static t_token	*new_token(t_token_type type, char **q, char **eq)
{
	t_token	*new_token;

	new_token = calloc(1, sizeof(t_token));
	if (!new_token)
		fatal_error("calloc");
	new_token->type = type;
	if (strchr("a'\"", type))
	{
		if (!q || !eq || !*q)
			fatal_error("new_token");
		new_token->word = *q;
		new_token->end = *eq;
		if (!new_token->word)
			fatal_error("strndup");
	}
	return (new_token);
}

static void	insert_null_terminator(t_token *token)
{
	t_token	*cur_token;

	cur_token = token;
	while (cur_token)
	{
		if (cur_token->end)
			*cur_token->end = '\0';
		cur_token = cur_token->next;
	}
}

t_token	*lexer(char *s)
{
	t_token	head_token;
	t_token	*cur_token;
	char	*q;
	char	*eq;

	head_token.next = NULL;
	cur_token = &head_token;
	while (*s)
	{
		q = NULL;
		eq = NULL;
		cur_token->next = new_token(get_token_type(&s, &q, &eq), &q, &eq);
		if (!cur_token->next || cur_token->next->type == TK_PARSE_ERROR)
			assert_error("Unexpected Token");
		cur_token = cur_token->next;
	}
	cur_token->next = new_token(TK_EOF, NULL, NULL);
	insert_null_terminator(head_token.next);
	return (head_token.next);
}
