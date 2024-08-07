#include "ast.h"
#include "ft_hash.h"
#include "minishell.h"
#include "token.h"

void	free_argv(char **argv)
{
	int	i;

	if (argv == NULL)
	{
		return ;
	}
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_mgr_resources(t_mgr *mgr)
{
	if (mgr->token)
	{
		free_tokens(mgr->token);
	}
	if (mgr->cmd)
	{
		free_cmd(mgr->cmd);
	}
	if (mgr->env_table)
	{
		mgr->env_table->free(mgr->env_table);
	}
}
