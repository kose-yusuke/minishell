/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_merge_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:37:07 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:37:23 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"
#include "xlibc.h"

static bool	should_merge(t_word *word)
{
	if (!word || !word->next)
		return (false);
	if (!is_word_or_quoted_token(word->token)
		|| !is_word_or_quoted_token(word->next->token))
		return (false);
	return (word->token->next == word->next->token);
}

static void	merge_adjacent_words(t_word *word)
{
	t_word	*next_word;
	char	*new_word;
	size_t	len;

	next_word = word->next;
	len = ft_strlen(word->token->word) + ft_strlen(next_word->token->word);
	new_word = xmalloc(len + 1);
	strcpy(new_word, word->token->word);
	strcat(new_word, next_word->token->word);
	word->token = next_word->token;
	free(word->token->word);
	word->token->word = new_word;
	word->next = next_word->next;
	free(next_word);
}

void	merge_words(t_word *word_list)
{
	t_word	*word_to_check;

	word_to_check = word_list;
	while (word_to_check && word_to_check->next)
	{
		if (should_merge(word_to_check))
			merge_adjacent_words(word_to_check);
		else
			word_to_check = word_to_check->next;
	}
}
