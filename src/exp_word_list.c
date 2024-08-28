/* exp_var.c */
#include "error.h"
#include "expander.h"
#include "xlibc.h"

static char	*find_dollar(char **cur_ptr)
{
	char	*dollar_ptr;

	dollar_ptr = ft_strchr(*cur_ptr, '$');
	if (!dollar_ptr || *(dollar_ptr + 1) == '\0')
	{
		*cur_ptr = NULL;
		return (NULL);
	}
	if (*(dollar_ptr + 1) == '$')
	{
		// $$の展開 -> プロセスID（minishell対象外）
		*cur_ptr = dollar_ptr + 2;
		return (NULL);
	}
	return (dollar_ptr);
}

static void	replace_word_str(char **word, char *expanded_value, char *suffix)
{
	size_t	len;
	char	*new_word;

	len = ft_strlen(*word) + ft_strlen(expanded_value) + ft_strlen(suffix);
	new_word = xmalloc(len + 1);
	ft_strcpy(new_word, *word);
	ft_strlcat(new_word, expanded_value, len + 1); // バッファサイズとしてlen + 1を渡す
	ft_strlcat(new_word, suffix, len + 1);         // 同じくバッファサイズとしてlen + 1を渡す
	free(*word);
	*word = new_word;
}

static void	expand_variable(char **word, char **cur_ptr, t_mgr *mgr)
{
	char	*dollar_ptr;
	char	*new_word;
	char	*suffix;
	char	*expanded_value;
	size_t	updated_len;

	dollar_ptr = find_dollar(cur_ptr);
	if (!dollar_ptr)
		return ;
	expanded_value = get_expanded_value(dollar_ptr, &suffix, mgr);
	if (!expanded_value)
		return ;
	*dollar_ptr = '\0';
	updated_len = ft_strlen(*word) + ft_strlen(expanded_value);
	replace_word_str(word, expanded_value, suffix);
	if (*(dollar_ptr + 1) == '?')
		free(expanded_value);
	*cur_ptr = *word + updated_len;
}

static void	expand_word_token(t_token *word_token, t_mgr *mgr)
{
	char	*current_ptr;

	if (word_token->type != TK_WORD && word_token->type != TK_DQUOTE)
		return ;
	current_ptr = word_token->word;
	while (current_ptr && *current_ptr)
	{
		expand_variable(&(word_token->word), &current_ptr, mgr);
	}
}

void	expand_word_list(t_word *word_list, t_mgr *mgr)
{
	t_word	*word_to_expand;
	t_word	*next_word;

	word_to_expand = word_list;
	while (word_to_expand)
	{
		next_word = word_to_expand->next;
		expand_word_token(word_to_expand->token, mgr);
		split_word_token(word_to_expand);
		word_to_expand = next_word;
	}
	merge_words(word_list);
}
