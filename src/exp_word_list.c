/* expand.c - 展開処理 */
#include "error.h"
#include "expander.h"

static bool	is_continuous_token(t_word *word, t_word *next_word)
{
	if (!word || !word->token || !next_word || !next_word->token)
		return (false);
	return (word->token->next == next_word->token);
}

static t_word	*append_assignment_value(t_word *word, char *str)
{
	t_word	*new_word;

	new_word = new_word_node(TK_DQUOTE, str);
	if (!new_word)
	{
		return (NULL);
	}
	word->next = new_word;
	word->token->next = new_word->token;
	return (new_word);
}

static void	divide_assignment_word(t_word *word, char *eq_pos)
{
	t_word	*next_word;
	t_token	*next_token;
	t_word	*after_eq_word;
	char	*until_eq_str;

	if (!word || !word->token || *(eq_pos + 1) == '\0')
		return ;
	next_word = word->next;
	next_token = word->token->next;
	after_eq_word = append_assignment_value(word, eq_pos + 1);
	if (!after_eq_word)
		return ;
	eq_pos[1] = '\0';
	until_eq_str = ft_strdup(word->token->word);
	free(word->token->word);
	word->token->word = until_eq_str;
	after_eq_word->next = next_word;
	after_eq_word->token->next = next_token;
}

static bool	process_assignment_word(t_word *word, bool after_equal_sign)
{
	char	*eq_pos;
	char	*dollar_pos;

	eq_pos = ft_strchr(word->token->word, '=');
	dollar_pos = ft_strchr(word->token->word, '$');
	if (!after_equal_sign && !eq_pos)
	{
		return (false);
	}
	if (after_equal_sign && !eq_pos && dollar_pos)
	{
		word->token->type = TK_DQUOTE;
	}
	if (eq_pos && *(eq_pos + 1) != '\0' && dollar_pos && eq_pos < dollar_pos)
	{
		divide_assignment_word(word, eq_pos);
	}
	return (is_continuous_token(word, word->next));
}

void	expand_word_list(t_word *word_list, t_mgr *mgr)
{
	t_word	*word;
	bool	has_export_cmd;
	bool	after_equal_sign;

	word = word_list;
	has_export_cmd = (ft_strcmp(word->token->word, "export") == 0);
	after_equal_sign = false;
	while (word)
	{
		if (word->token->type == TK_WORD && has_export_cmd)
		{
			after_equal_sign = process_assignment_word(word, after_equal_sign);
		}
		if (word->token->type == TK_WORD || word->token->type == TK_DQUOTE)
		{
			expand_word_str(&(word->token->word), mgr);
		}
		split_word_token(word);
		word = word->next;
	}
	merge_words(word_list);
}
