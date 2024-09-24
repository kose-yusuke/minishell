/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:54:15 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/24 21:09:52 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "error.h"

int	builtin_unset(char **argv, t_mgr *mgr)
{
	int		i;
	int		status;
	char	*str;

	i = 1;
	status = 0;
	while (argv[i])
	{
		if (unset_env(&mgr->env_list, argv[i]) == -1)
		{
			str = prepare_str_to_print(argv[i]);
			report_error("unset", str, "not a valid identifier");
			free(str);
			status = 1;
		}
		i++;
	}
	return (status);
}
