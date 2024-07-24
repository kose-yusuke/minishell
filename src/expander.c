/* expand.c - 展開処理 */
#include "expander.h"
#include "minishell.h"

char	**ft_split_multidelim(char *str, const char *delimiters)
{
	char	**splitted;
	size_t	i;

	i = 0;
	// ft_splitで複数の区切り文字を指定することができないため、区切り文字をスペースに変換する。
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

void	free_2d_array(void **array)
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

// 改装中
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

char	*extract_env_key(char *env_head, char **env_tail)
{
	char	*env_key;

	if (!env_head || !*env_head)
		return (NULL);
	*env_tail = env_head;
	while (**env_tail && !strchr(IFS, **env_tail))
		(*env_tail)++;
	env_key = strndup(env_head, *env_tail - env_head);
	if (!env_key)
	{
		report_error("expand_env", NULL, "strndup failed");
		return (NULL);
	}
	return (env_key);
}

void	expand_env(t_token *word_token, char **cur_ptr, t_hash_table *env_table)
{
	char	*dollar_ptr;
	char	*env_key;
	char	*env_tail;
	char	*value;
	size_t	len;
	char	*new_word;

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
	if (*(dollar_ptr + 1) == '?')
	{
		// $?の処理 -> 直前のコマンドの終了ステータスに置き換える
		*cur_ptr = dollar_ptr + 2;
		return ;
	}
	env_key = extract_env_key(dollar_ptr + 1, &env_tail);
	if (!env_key)
	{
		*cur_ptr = NULL; //  TODO: error handling
		return ;
	}
	value = env_table->search(env_table, env_key);
	free(env_key);
	if (value == NULL)
	{
		// 環境変数が見つからない場合の処理
		memmove(dollar_ptr, env_tail, strlen(env_tail) + 1);
		// TODO: ft_memmoveに変更
		*cur_ptr = dollar_ptr;
		// 次の展開処理を続けるためにcur_ptrを更新
		return ;
	}
	*dollar_ptr = '\0';
	// env_tailにはenv_keyが終了した位置（IFSまたは文字列の終端）が入っている
	len = strlen(word_token->word) + strlen(value) + strlen(env_tail);
	// TODO: ft_strlenに変更
	new_word = calloc(len + 1, sizeof(char));
	if (!new_word)
	{
		report_error("expand_env", NULL, "calloc failed");
		// TODO: error handling
		*cur_ptr = NULL;
		return ;
	}
	// TODO: この辺も、のちにft_strcpy, ft_strcatに変更が必要
	strcpy(new_word, word_token->word);
	strcat(new_word, value);
	strcat(new_word, env_tail);
	free(word_token->word);
	word_token->word = new_word;
	*cur_ptr = dollar_ptr + strlen(value); // ?
}

void	expand_word_token(t_token *word_token, t_hash_table *env_table)
{
	char	*current_ptr;

	if (word_token->type != TK_WORD && word_token->type != TK_DQUOTE)
		return ;
	current_ptr = word_token->word;
	while (current_ptr && *current_ptr)
	{
		expand_env(word_token, &current_ptr, env_table);
	}
}

void	expand_word_list(t_word *word_list, t_hash_table *env_table)
{
	t_word	*word_to_expand;
	t_word	*next_word;

	word_to_expand = word_list;
	while (word_to_expand)
	{
		next_word = word_to_expand->next;
		expand_word_token(word_to_expand, env_table);
		word_splitting(word_to_expand);
		word_to_expand = next_word;
	}
}

void	expand_redir_list(t_redir *redir_list, t_hash_table *env_table)
{
	while (redir_list)
	{
		if (redir_list->redir_type == TK_HEREDOC)
		{
			/*
				NOTE: bashにおけるheredocの挙動について
					$ cat <<EOF        <- heredocは変数展開される
					$ cat <<'EOF'      <- heredocは変数展開されない
					$ cat <<"EOF"      <- heredocは変数展開されない
					$ cat <<$EOF       <- デリミタは変数展開されず、heredocは変数展開される
					$ cat <<"$EOF"     <- デリミタは変数展開されず、heredocは変数展開されない

				いずれの場合も、heredocの中身がword splittingされることはない

				デリミタがquoteをまたいで、複数のtokenにまたがる場合は
				heredoc実行の前にデリミタがmergeされていなければならない

				ただしデリミタがquoteを含むかどうかは、heredocの変数展開に必要な情報なので
				mergeされる際には、quoteが含まれるかどうかを保持しておく必要がある
				TODO: あとで実装
			*/
		}
		else
		{
			expand_word_list(redir_list->word_list, env_table);
		}
		redir_list = redir_list->next;
	}
}

void	run_expansion(t_cmd *cmd, t_hash_table *env_table)
{
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;

	if (!cmd || cmd->type == NONE || !env_table)
		return ;
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		expand_word_list(ecmd->word_list, env_table);
		expand_redir_list(ecmd->redir_list, env_table);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		run_expansion(pcmd->left, env_table);
		run_expansion(pcmd->right, env_table);
	}
	else
		exit(EXIT_FAILURE);
}
/*
note: exapansionの対象となるのは以下の構造体におけるword_listとredir_listである

typedef struct s_token
{
	enum e_token_type	type;
	char				*word;
	struct s_token		*next;
}						t_token;

typedef struct s_word
{
	struct s_token		*token;
	struct s_word		*next;
}						t_word;

typedef struct s_redir
{
	enum e_token_type	redir_type;
	int					fd;
	int					backup_fd;
	struct s_word		*word_list;
	struct s_redir		*next;
}						t_redir;

typedef enum e_cmd_type
{
	NONE,
	EXEC,
	PIPE
}						t_cmd_type;

typedef struct s_cmd
{
	enum e_cmd_type		type;
}						t_cmd;

typedef struct s_execcmd
{
	enum e_cmd_type		type;
	struct s_word		*word_list;
	struct s_redir		*redir_list;
	char				*eof_word;
}						t_execcmd;

typedef struct s_pipecmd
{
	enum e_cmd_type		type;
	struct s_cmd		*left;
	struct s_cmd		*right;
}						t_pipecmd;
 */
