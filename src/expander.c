/* expand.c - 展開処理 */
#include "expander.h"

/*
### minishellに必要な展開

 **パラメータと変数の展開 (parameter and variable expansion)**：
   - `$` を用いて環境変数やシェル変数を展開
   - `'`で囲まれた文字列は展開されない
   - `"`で囲まれた文字列は展開されるが、分割されない
   - 囲まれていない文字列は展開され、スペースまたはタブで分割される

 **単語の分割 (word splitting)**：
   - IFS に基づいて、展開された結果を単語に分割 IFS=" \t\n" と設定

### 自信のないポイント
 - minishellにおいて環境変数とシェル変数をどのように扱うべきなのか。一緒にしてもいいのか。
 - hash_tableを使って環境変数を管理するのか、getenv()を中心に使うのか。
 */

#define IFS " \t\n" // TODO: move to header file

void	expand_env_var(t_word *word_list, t_hash_table *env_table)
{
	char	*dollar_ptr;
	char	*env_key;
	char	*env_tail;
	char	*value;
	size_t	len;
	char	*new_word;

	if (word_list->token->type != TK_WORD
		&& word_list->token->type != TK_DQUOTE)
		return ;
	dollar_ptr = strchr(word_list->token->word, '$');
	if (!dollar_ptr || *(dollar_ptr + 1) == '\0')
		return ;
	env_tail = dollar_ptr + 1;
	while (*env_tail && !strchr(IFS, *env_tail))
		env_tail++;
	// ここの計算が怪しい + strtrim()を使うべきかも
	env_key = strndup(dollar_ptr + 1, env_tail - dollar_ptr - 1);
	if (!env_key)
	{
		// TODO: error handling
		return ;
	}
	value = env_table->search(env_table, env_key);
	free(env_key);
	if (value == NULL)
	{
		// 環境変数が見つからない場合の処理
		memmove(dollar_ptr, env_tail, strlen(env_tail) + 1);
		return ;
	}
	*dollar_ptr = '\0';
	// ここの計算が怪しい
	len = strlen(word_list->token->word) + strlen(value) + strlen(env_tail);
	new_word = calloc(len + 1, sizeof(char));
	if (!new_word)
	{
		// TODO: error handling
	}
	strcpy(new_word, word_list->token->word);
	strcat(new_word, value);
	strcat(new_word, env_tail);
	if (word_list->token->allocated)
		free(word_list->token->word); // 古いメモリを解放
	word_list->token->word = new_word;
	word_list->token->allocated = true;
}

void	word_splitting(t_word *word_list)
{
	t_word	head_to_throw;
	t_word	*added_word_list;
	char	*word_to_split;

	if (word_list->token->type != TK_WORD)
		return ;
	added_word_list = &head_to_throw;
	word_to_split = word_list->token->word;
	while (*word_to_split)
	{
		if (!strchr(IFS, *word_to_split))
		{
			word_to_split++;
			continue ;
		}
		*word_to_split = '\0';
		word_to_split++;
		while (*word_to_split && strchr(IFS, *word_to_split))
			word_to_split++;
		if (*word_to_split == '\0')
			break ;
		added_word_list->next = calloc(1, sizeof(t_word));
		if (!added_word_list->next)
		{
			// TODO: error handling
			perror_exit("calloc");
		}
		added_word_list = added_word_list->next;
		added_word_list->token = new_token(TK_WORD, &word_to_split, NULL);
		if (!added_word_list->token)
		{
			// TODO: error handling
			perror_exit("new_token");
		}
		added_word_list->next = NULL;
	}
	if (head_to_throw.next)
	{
		added_word_list->token->next = word_list->token->next;
		added_word_list->next = word_list->next;
		word_list->token->next = head_to_throw.next->token;
		word_list->next = head_to_throw.next;
	}
}

// ダミー関数
void	expand_word_list(t_word *word_list, t_hash_table *env_table)
{
	t_word	*next_word;

	// ここに環境変数やチルダの展開の実装を追加
	next_word = NULL;
	while (word_list)
	{
		next_word = word_list->next;
		expand_env_var(word_list, env_table);
		word_splitting(word_list);
		word_list = next_word;
	}
}

void	expand_redir_list(t_redir *redir_list, t_hash_table *env_table)
{
	// TODO : here documentの処理を追加
	while (redir_list)
	{
		expand_word_list(redir_list->word_list, env_table);
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

void	run_expansion(t_cmd *cmd, t_hash_table *env_table)
{
	if (!cmd || cmd->type == NONE || !env_table)
		return ;
	else if (cmd->type == EXEC)
		expand_exec((t_execcmd *)cmd, env_table);
	else if (cmd->type == PIPE)
		expand_pipe((t_pipecmd *)cmd, env_table);
	else
		exit(EXIT_FAILURE);
}
