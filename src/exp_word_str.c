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

void	expand_word_str(char **word, t_mgr *mgr)
{
	char	*current_ptr;

	if (!word || !*word)
		return ;
	current_ptr = *word;
	while (current_ptr && *current_ptr)
	{
		expand_variable(word, &current_ptr, mgr);
	}
}
