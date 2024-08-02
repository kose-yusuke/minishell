#include "minishell.h"

/**
 * `$?`は直前のexit statusが必要なので、exec段階で展開する
 * ただし、パイプライン内の途中のstatusは取得できず、反映されない
 * Minishellでは`;`を実装しないため、実は`$?`もexpand段階で展開可能
 */
void	expand_exit_status(char **word, char **cur_ptr, int exit_status)
{
	char	*dollar_ptr;
	char	*status_str;
	size_t	len;
	char	*new_word;
	size_t	updated_len;

	dollar_ptr = strchr(*cur_ptr, '$'); // TODO: ft_strchrに変更
	if (!dollar_ptr || *(dollar_ptr + 1) == '\0')
	{
		*cur_ptr = NULL;
		return ;
	}
	if (*(dollar_ptr + 1) == '$')
	{
		// $$の処理 -> 本来はプロセスIDに置き換えるがminishellでは対象外
		*cur_ptr = dollar_ptr + 2;
		return ;
	}
	if (*(dollar_ptr + 1) != '?')
	{
		// 他の`$ENV`は事前のexpand段階で対応済み。エラー処理が必要かは不明
		*cur_ptr = dollar_ptr + 1;
		return ;
	}
	*dollar_ptr = '\0';
	status_str = ft_itoa(exit_status);
	len = ft_strlen(*word) + ft_strlen(status_str) + ft_strlen(dollar_ptr + 2);
	updated_len = ft_strlen(*word) + ft_strlen(status_str);
	new_word = calloc(len + 1, sizeof(char));
	if (!new_word)
	{
		// TODO: error handling
		report_error("expand_exit_status", NULL, strerror(errno));
		*cur_ptr = NULL;
		return ;
	}
	// TODO: あとでft_strcpy, ft_strcatに変更が必要
	strcpy(new_word, *word);
	strcat(new_word, status_str);
	strcat(new_word, dollar_ptr + 2);
	free(*word);
	free(status_str);
	*word = new_word;
	*cur_ptr = *word + updated_len;
}

static void	expand_word_token_for_exit_status(t_token *word_token,
		int exit_status)
{
	char	*current_ptr;

	if (word_token->type != TK_WORD && word_token->type != TK_DQUOTE)
		return ;
	current_ptr = word_token->word;
	while (current_ptr && *current_ptr)
	{
		expand_exit_status(&(word_token->word), &current_ptr, exit_status);
	}
}

void	expand_word_list_for_exit_status(t_word *word_list, int exit_status)
{
	t_word *word_to_expand;

	word_to_expand = word_list;
	while (word_to_expand)
	{
		expand_word_token_for_exit_status(word_to_expand->token, exit_status);
		word_to_expand = word_to_expand->next;
	}
}
