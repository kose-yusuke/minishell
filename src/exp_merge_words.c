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
	// 1stが2ndのtokenを指す
	word->token = next_word->token;
	// 旧wordのメモリを解放
	free(word->token->word);
	// 2ndのtokenのwordを新しいwordに設定
	word->token->word = new_word;
	// 1stのnextを3rdに設定
	word->next = next_word->next;
	// 2ndのt_word構造体を解放
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
