/* expand.c - 展開処理 */
#include "expander.h"
#include "minishell.h"

void	expand_env_var(t_word *word_list, t_mgr *mgr)
{
	char	*dollar_ptr;
	char	*env_key;
	char	*env_tail;
	char	*value;
	size_t	len;
	char	*new_word;

	dollar_ptr = strchr(word_list->token->word, '$');
	if (!dollar_ptr || *(dollar_ptr + 1) == '\0')
		return ;
	env_tail = dollar_ptr + 1;
	while (*env_tail && !strchr(IFS, *env_tail))
		env_tail++;
	env_key = strndup(dollar_ptr + 1, env_tail - dollar_ptr - 1);
	if (!env_key)
	{
		report_error("expand_env_var", NULL, "strndup failed");
		mgr->status = -1;
		return ;
	}
	value = mgr->env_table->search(mgr->env_table, env_key);
	free(env_key);
	if (value == NULL)
	{
		// 環境変数が見つからない場合の処理
		memmove(dollar_ptr, env_tail, strlen(env_tail) + 1);
		return ;
	}
	*dollar_ptr = '\0';
	len = strlen(word_list->token->word) + strlen(value) + strlen(env_tail);
	new_word = calloc(len + 1, sizeof(char));
	if (!new_word)
	{
		report_error("expand_env_var", NULL, "calloc failed");
		mgr->status = -1;
		return ;
	}
	strcpy(new_word, word_list->token->word);
	strcat(new_word, value);
	strcat(new_word, env_tail);
	free(word_list->token->word);
	word_list->token->word = new_word;
}

void	expand_var_loop(t_word *word_list, t_mgr *mgr)
{
	t_word	*word_to_expand;
	t_word	*next;

	word_to_expand = word_list;
	if (word_list->token->type == TK_WORD)
	{
		expand_env_var(word_to_expand, mgr);
		return ;
	}
	if (word_list->token->type == TK_DQUOTE)
	{
		while (strchr(word_to_expand->token->word, '$'))
		{
			expand_env_var(word_to_expand, mgr);
		}
	}
}

char	**ft_split_multidelim(char *str, const char *delimiters)
{
	char	**splitted;
	size_t	i;

	i = 0;
	// ft_splitで複数の区切り文字を指定することができないため、区切り文字をスペースに変換する。
	// （ft_splitの改良版をちゃんと作成した際には削除する）
	while (str[i])
	{
		if (strchr(delimiters, str[i]))
		{
			str[i] = ' ';
		}
		i++;
	}
	splitted = ft_split(str, ' ');
	return (splitted);
}

void	word_splitting(t_word *word_list)
{
	char	**splitted;
	char	**q;
	char	**eq;
	size_t	i;
	t_word	*next_word;
	t_token	*next_token;

	if (word_list->token->type != TK_WORD || !word_list->token->word
		|| !word_list->token->word[0])
		return ;
	splitted = ft_split_multidelim(word_list->token->word, IFS);
	if (!splitted)
	{
		assert_error("ft_split_multidelim failed", "word_splitting");
	}
	next_word = word_list->next;
	next_token = word_list->token->next;
	word_list->token->word = splitted[0];
	word_list->token->next = NULL; // 必要？
	word_list->next = NULL;        // 必要？
	i = 1;
	while (splitted[i])
	{
		q = &splitted[i];
		eq = &splitted[i] + strlen(splitted[i]);
		word_list->next = calloc(1, sizeof(t_word));
		if (!word_list->next)
		{
			free_2d_array((void **)splitted);
			assert_error("calloc failed", "word_splitting");
		}
		word_list->next->token = new_token(TK_WORD, q, eq);
		if (!word_list->next->token)
		{
			free_2d_array((void **)splitted);
			assert_error("new_token failed", "word_splitting");
		}
		word_list->token->next = word_list->next->token;
		word_list = word_list->next;
		i++;
	}
	free_2d_array((void **)splitted);
	word_list->next = next_word;
	word_list->token->next = next_token;
}

/*

TMP_FUNC_NAME は関数名を一時的につけたもので、適切な関数名に変更する必要がある

	`aaa"ccccc"bbb` という文字列がある場合、`aaa` と `ccccc` と `bbb` にtokenとして
	分割されて、それぞれwordtoken, quotedtoken, wordtokenとなる。
	トークンとして分割された状態で展開の処理を経て、最終的には
	統合して `aaaccccbbb` という文字列になりたいので、くっつける関数が必要になる
	*/

bool	should_merge(t_word *word)
{
	t_word	*next_word;

	next_word = word->next;
	if (!word || !next_word)
		return (false);
	if (is_word_or_quoted_token(word->token)
		&& is_word_or_quoted_token(next_word->token))
		return (word->token->next == next_word->token);
}

void	merge_adjacent_words(t_word *word)
{
	t_word	*next_word;
	t_word	*next_next_word;
	t_token	*next_next_token;
	char	*new_word;
	size_t	len;

	next_word = word->next;
	len = strlen(word->token->word) + strlen(next_word->token->word) + 1;
	new_word = calloc(len, sizeof(char));
	if (!new_word)
	{
		// TODO: error handling
		assert_error("calloc failed", "merge_adjacent_words");
	}
	strcpy(new_word, word->token->word);
	strcat(new_word, next_word->token->word);
	free(word->token->word);
	word->token->word = new_word;
	next_next_word = next_word->next;
	next_next_token = next_next_word->token;
	free(next_word->token->word);
	free(next_word->token);
	free(next_word);
	word->next = next_next_word;
	word->token->next = next_next_token;
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

void	expand_word_list(t_word *word_list, t_hash_table *env_table)
{
	t_word	*word_to_expand;
	t_word	*next;

	word_to_expand = word_list;
	while (word_to_expand)
	{
		next = word_to_expand->next;
		expand_env_loop(word_to_expand, env_table);
		word_splitting(word_to_expand);
		word_to_expand = next;
	}
	merge_words(word_list);
}

void	expand_redir_list(t_redir *redir_list, t_hash_table *env_table)
{
	while (redir_list)
	{
		if (redir_list->redir_type == TK_HEREDOC)
		{
			merge_words(redir_list->word_list);
			// TODO : here documentの処理を追加
		}
		else
		{
			expand_word_list(redir_list->word_list, env_table);
		}
		redir_list = redir_list->next;
	}
}

void	expand_exec(t_execcmd *ecmd, t_hash_table *env_table)
{
	expand_word_list(ecmd->word_list, env_table);
	expand_redir_list(ecmd->redir_list, env_table);
}

void	expand_pipe(t_pipecmd *pcmd, t_hash_table *env_table)
{
	run_expansion(pcmd->left, env_table);
	run_expansion(pcmd->right, env_table);
}

void	run_expansion(t_cmd *cmd, t_mgr *mgr)
{
	if (!cmd || cmd->type == NONE || !mgr->env_table)
		return ;
	else if (cmd->type == EXEC)
		expand_exec((t_execcmd *)cmd, mgr->env_table);
	else if (cmd->type == PIPE)
		expand_pipe((t_pipecmd *)cmd, mgr->env_table);
	else
		exit(EXIT_FAILURE);
}

