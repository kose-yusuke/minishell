/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_word_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 00:41:54 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/05 00:46:59 by sakitaha         ###   ########.fr       */
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

	token = new_token(TK_WORD, &str, &str + ft_strlen(str));
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

static bool	set_first_word(t_word *word, char *str_to_replace)
{
	char	*first_str;

	first_str = ft_strdup(str_to_replace);
	if (!first_str)
	{
		return (false);
	}
	free(word->token->word);
	word->token->word = first_str;
	return (true);
}

static t_word	*insert_remaining_words(t_word *word, char **splits)
{
	t_word	*current_word;
	size_t	i;

	current_word = word;
	i = 1;
	while (splits[i])
	{
		current_word->next = insert_word(word, splits[i]);
		if (!current_word->next)
			return (NULL);
		current_word = current_word->next;
		i++;
	}
	return (current_word);
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
	if (!set_first_word(word, splits[0]))
	{
		free_argv(splits);
		return ;
	}
	last_word = insert_remaining_words(word, splits);
	free_argv(splits);
	if (!last_word)
		return ;
	last_word->next = next_word;
	last_word->token->next = next_token;
}
