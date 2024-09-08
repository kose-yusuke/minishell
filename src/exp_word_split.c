/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_word_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 00:41:54 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/08 01:29:02 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"
#include "free.h"
#include "parser.h"
#include "xlibc.h"

static t_word	*new_word_node(char *str)
{
	t_token	*token;
	char	*start;
	char	*end;

	start = str;
	end = str + ft_strlen(str);
	token = new_token(TK_WORD, &start, &end);
	if (!token)
		return (NULL);
	return (init_word(token));
}

static t_word	*insert_word(t_word *prev_word, char *str)
{
	t_token	*blank_token;
	t_word	*new_word;

	blank_token = new_token(TK_BLANK, NULL, NULL);
	if (!blank_token)
		return (NULL);
	new_word = new_word_node(str);
	if (!new_word)
	{
		free(blank_token);
		return (NULL);
	}
	prev_word->next = new_word;
	prev_word->token->next = blank_token;
	blank_token->next = new_word->token;
	return (new_word);
}

static bool	set_first_word(t_word *word, char *str)
{
	char	*first_str;

	first_str = ft_strdup(str);
	if (!first_str)
		return (false);
	free(word->token->word);
	word->token->word = first_str;
	return (true);
}

static t_word	*set_words(t_word *word, char **splits)
{
	t_word	*prev_word;
	size_t	i;

	i = 0;
	if (!set_first_word(word, splits[i]))
		return (NULL);
	i++;
	prev_word = word;
	while (splits[i])
	{
		prev_word->next = insert_word(prev_word, splits[i]);
		if (!prev_word->next)
			return (NULL);
		prev_word = prev_word->next;
		i++;
	}
	return (prev_word);
}

void	split_word_token(t_word *word)
{
	t_word	*next_word;
	t_token	*next_token;
	t_word	*last_word;
	char	**splits;

	if (!word || !word->token || !has_delimiter(word->token, IFS))
		return ;
	next_word = word->next;
	next_token = word->token->next;
	splits = ft_split(word->token->word, ' ');
	if (!splits)
		return ;
	last_word = set_words(word, splits);
	free_argv(splits);
	if (!last_word)
		return ;
	last_word->next = next_word;
	last_word->token->next = next_token;
}
