/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 00:35:15 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/05 00:47:36 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "utils.h"

bool	has_delimiter(t_token *token, const char *delimiters)
{
	char	*str;
	bool	found_delimiter;

	if (!is_word_token(token) || !token->word || !token->word[0])
		return (false);
	str = token->word;
	found_delimiter = false;
	while (*str)
	{
		if (ft_strchr(delimiters, *str))
		{
			*str = ' ';
			found_delimiter = true;
		}
		str++;
	}
	return (found_delimiter);
}
