/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:10:34 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/24 03:51:51 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"
#include "minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		if (is_word_or_quoted_token(token) || is_io_num_token(token))
		{
			free(token->word);
			token->word = NULL;
		}
		free(token);
		token = next;
	}
}

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
	if (mgr->env_list)
	{
		free_env_list(mgr->env_list);
	}
}
