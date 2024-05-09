#include "minishell.h"
#include "token.h"

void	free_tokens(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		free(token);
		token = next;
	}
}

// koseki-san
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
