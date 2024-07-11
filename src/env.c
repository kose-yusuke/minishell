/* env.c - 環境変数の操作に関連する関数の実装 */
#include "env.h"
#include "ft_hash.h"

t_hash_table	*create_env_table(void)
{
	extern char		**environ;
	t_hash_table	*env_table;
	char			*env;
	char			*delimiter;
	size_t			i;

	env_table = create_hash_table();
	if(!env_table)
	{
		return (NULL);
	}
	i = 0;
	while (environ[i])
	{
		env = strdup(environ[i]);
		if (!env)
		{
			free_hash_table(env_table);
			return (NULL);
		}
		delimiter = strchr(env, '=');
		if (delimiter) // if(delimiter && *delimiter) ？
		{
			*delimiter = '\0';
			env_table->insert(env_table, env, delimiter + 1);
		}
		free(env);
		i++;
	}
	return (env_table);
}
