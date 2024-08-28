/* exp_var_utils.c */

#include "error.h"
#include "expander.h"
#include "xlibc.h"

static char	*extract_env_key(char *env_head, char **suffix)
{
	char	*env_key;
	size_t	len;

	*suffix = env_head;
	// suffixにはenv_keyの終了位置（IFSまたは文字列の終端）が入る
	// TODO: strchr をft_strchrに変更
	while (**suffix && !strchr(IFS, **suffix) && **suffix != '$')
		(*suffix)++;
	len = *suffix - env_head;
	env_key = xmalloc(len + 1);
	ft_strncpy(env_key, env_head, len);
	env_key[len] = '\0';
	return (env_key);
}

static char	*expand_exit_status(char *dollar_ptr, t_status exit_status)
{
	char	*expanded_value;

	expanded_value = ft_itoa(exit_status);
	if (!expanded_value)
	{
		memory_error_and_exit("ft_itoa");
		return (NULL);
	}
	return (expanded_value);
}

static char	*expand_env(char *dollar_ptr, char **suffix,
		t_hash_table *env_table)
{
	char	*env_key;
	char	*expanded_value;

	env_key = extract_env_key(dollar_ptr + 1, suffix);
	expanded_value = env_table->search(env_table, env_key);
	free(env_key);
	return (expanded_value);
}

char	*get_expanded_value(char *dollar_ptr, char **suffix, t_mgr *mgr)
{
	char	*expanded_value;

	if (*(dollar_ptr + 1) == '?')
	{
		expanded_value = expand_exit_status(dollar_ptr, mgr->exit_status);
		*suffix = dollar_ptr + 2;
		return (expanded_value);
	}
	expanded_value = expand_env(dollar_ptr, suffix, mgr->env_table);
	if (!expanded_value)
	{
		ft_strcpy(dollar_ptr, *suffix);
		return (NULL);
	}
	return (expanded_value);
}
