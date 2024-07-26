/* lexer.c - 入力をトークンに分割する字句解析器の実装。 */
#include "lexer.h"

int	ft_isdigit(int c)
{
	while (c > 47 && c < 58)
	{
		return (1);
	}
	return (0);
}

static t_token_type	get_blank_type(char **ps)
{
	char	*s;

	s = *ps;
	if (!s || !*s)
		return (TK_UNDEF_TOKEN);
	if (*s == '\n')
	{
		(*ps)++;
		return (TK_NL);
	}
	if (*s == ' ' || *s == '\t')
	{
		while (*s == ' ' || *s == '\t')
			s++;
		*ps = s;
		return (TK_BLANK);
	}
	return (TK_UNDEF_TOKEN);
}

static t_token_type	get_op_type(char **ps)
{
	static const char			*op[] = {"|", "<<", ">>", "<", ">", NULL};
	static const t_token_type	tok[] = {TK_PIPE, TK_HEREDOC, TK_APPEND,
			TK_REDIR_IN, TK_REDIR_OUT};
	static const size_t			len[] = {1, 2, 2, 1, 1};
	size_t						i;

	i = 0;
	if (!*ps || !**ps)
		return (TK_UNDEF_TOKEN);
	while (op[i])
	{
		if (strncmp(*ps, op[i], len[i]) == 0)
		{
			*ps += len[i];
			return (tok[i]);
		}
		i++;
	}
	return (TK_UNDEF_TOKEN);
}

static t_token_type	get_quoted_type(char **ps, char **q, char **eq)
{
	char	*s;
	char	quote_char;

	s = *ps;
	quote_char = *s;
	if (quote_char != '\'' && quote_char != '"')
		return (TK_UNDEF_TOKEN);
	*q = ++s;
	while (*s && (*s != quote_char || (quote_char == '"' && *(s - 1) == '\\')))
		s++;
	if (*s != quote_char)
		return (TK_PARSE_ERROR);
	*eq = s;
	*ps = s + 1; // 次のトークンの開始位置
	if (quote_char == '\'')
		return (TK_SQUOTE);
	else
		return (TK_DQUOTE);
}

static bool	is_meta_character(char c)
{
	static const char	*meta_character = "|<> '\" \t\n";

	return (strchr(meta_character, c) != 0);
}

static t_token_type	get_word_or_ionum_type(char **ps, char **q, char **eq)
{
	char	*s;
	bool	is_num;

	s = *ps;
	*q = s;
	is_num = true;
	if (!*s || is_meta_character(*s))
		return (TK_PARSE_ERROR);
	while (*s && !is_meta_character(*s))
	{
		if (!ft_isdigit(*s))
			is_num = false;
		s++;
	}
	*eq = s;
	*ps = s; // 次のトークンの開始位置
	if (is_num && (*s == '<' || *s == '>'))
		return (TK_IO_NUM);
	return (TK_WORD);
}

static t_token_type	get_token_type(char **ps, char **q, char **eq)
{
	t_token_type	type;

	type = get_blank_type(ps);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	type = get_op_type(ps);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	type = get_quoted_type(ps, q, eq);
	if (type != TK_UNDEF_TOKEN)
		return (type);
	return (get_word_or_ionum_type(ps, q, eq));
}

t_token	*new_token(t_token_type type, char **q, char **eq)
{
	t_token	*new_token;
	size_t	len;

	if (type == TK_UNDEF_TOKEN || type == TK_PARSE_ERROR)
		return (NULL);
	new_token = calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	if (is_word_or_quoted_token(new_token) || is_io_num_token(new_token))
	{
		if (!q || !eq || !*q)
		{
			free(new_token);
			report_error("new_token", NULL, "invalid arguments"); // ?
			return (NULL);
		}
		len = *eq - *q;
		new_token->word = ft_strndup(*q, len);
		if (!new_token->word)
		{
			free(new_token);
			report_error("new_token", NULL, "strndup failed"); // ?
			return (NULL);
		}
	}
	return (new_token);
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
		if (!cur_token->next)
		{
			free_tokens(head_token.next);
			return (NULL);
		}
		cur_token = cur_token->next;
	}
	cur_token->next = new_token(TK_EOF, NULL, NULL);
	if (!cur_token->next)
	{
		free_tokens(head_token.next);
		return (NULL);
	}
	return (head_token.next);
}
