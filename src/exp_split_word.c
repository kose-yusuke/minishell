#include "error.h"
#include "expander.h"
#include "minishell.h"

// TODO: refactor
/**
 * word トークンを分割するかどうかを判定する関数
 *  */
static bool	should_split(t_word *word, const char *delimiters)
{
	bool	has_delimiter;
	size_t	i;

	if (!word || !word->token || word->token->type != TK_WORD
		|| !word->token->word || !word->token->word[0])
		return (false);
	i = 0;
	has_delimiter = false;
	while (word->token->word[i])
	{
		if (strchr(delimiters, word->token->word[i]))
		{
			word->token->word[i] = ' ';
			has_delimiter = true;
		}
		i++;
	}
	return (has_delimiter);
}

/**
 * 新しい word トークンを作成する関数
 */
static t_word	*new_word_token(char *str)
{
	t_word	*new_word;

	new_word = calloc(1, sizeof(t_word));
	if (!new_word)
	{
		assert_error("calloc failed", "new_word_token");
		return (NULL);
	}
	new_word->token = new_token(TK_WORD, &str, &str + strlen(str));
	if (!new_word->token)
	{
		free(new_word);
		assert_error("new_token failed", "new_word_token");
		return (NULL);
	}
	return (new_word);
}

/**
 * 新しい word トークンを追加し、前のトークンとリンクさせる関数
 * */
static t_word	*add_word_token(t_word *prev_word, char *str)
{
	t_word	*new_word;
	t_token	*delimi_blank;

	new_word = new_word_token(str);
	if (!new_word)
		return (NULL);
	delimi_blank = new_token(TK_BLANK, NULL, NULL);
	if (!delimi_blank)
	{
		free(new_word->token->word);
		free(new_word->token);
		free(new_word);
		assert_error("new_token failed", "add_word_token");
		return (NULL);
	}
	prev_word->next = new_word;
	prev_word->token->next = delimi_blank;
	delimi_blank->next = new_word->token;
	return (new_word);
}

/**
 * 2次元配列を解放する関数
 *  */
static void	free_2d_array(void **array)
{
	size_t	i;

	if (array == NULL)
		return ;
	for (i = 0; array[i] != NULL; i++)
	{
		free(array[i]);
	}
	free(array);
}

/**
 * wordトークンを分割する関数
 */
void	split_word_token(t_word *word)
{
	char	**splitted;
	t_word	*next_word_at_end;
	t_token	*next_token_at_end;
	size_t	i;

	if (!should_split(word, IFS))
		return ;
	// 次のsplitされた文字列から参照されるべきword/tokenを保持
	next_word_at_end = word->next;
	next_token_at_end = word->token->next;
	// word->token->wordをIFSで分割
	splitted = ft_split(word->token->word, ' ');
	if (!splitted)
	{
		assert_error("ft_split failed", "split_word_token");
		return ;
	}
	// 最初のsplitされた文字列をword->token->wordに設定
	free(word->token->word);
	word->token->word = strdup(splitted[0]);
	if (!word->token->word)
	{
		free_2d_array((void **)splitted);
		assert_error("strdup failed", "split_word_token");
		return ;
	}
	i = 1;
	while (splitted[i])
	{
		word->next = add_word_token(word, splitted[i]);
		if (!word->next)
		{
			assert_error("add_word_token failed", "split_word_token");
			break ;
		}
		word = word->next;
		i++;
	}
	free_2d_array((void **)splitted);
	word->next = next_word_at_end;
	word->token->next = next_token_at_end;
}
