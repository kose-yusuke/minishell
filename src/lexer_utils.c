/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:17:32 by sakitaha          #+#    #+#             */
/*   Updated: 2024/10/08 14:39:28 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token_type	get_blank_type(char **ps)
{
	char	*s;

	s = *ps;
	if (!s || !*s)
		return (TK_UNDEF_TOKEN);
	if (*s != ' ' && *s != '\t')
		return (TK_UNDEF_TOKEN);
	while (*s == ' ' || *s == '\t')
		s++;
	*ps = s;
	return (TK_BLANK);
}

t_token_type	get_op_type(char **ps)
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
		if (ft_strncmp(*ps, op[i], len[i]) == 0)
		{
			*ps += len[i];
			return (tok[i]);
		}
		i++;
	}
	return (TK_UNDEF_TOKEN);
}

t_token_type	get_quoted_type(char **ps, char **q, char **eq)
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
	*ps = s + 1;
	if (quote_char == '\'')
		return (TK_SQUOTE);
	else
		return (TK_DQUOTE);
}

static bool	is_meta_character(char c)
{
	static const char	*meta_character = "|<> '\" \t\n";

	return (ft_strchr(meta_character, c) != 0);
}

t_token_type	get_word_or_ionum_type(char **ps, char **q, char **eq)
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
	*ps = s;
	if (is_num && (*s == '<' || *s == '>'))
		return (TK_IO_NUM);
	return (TK_WORD);
}
