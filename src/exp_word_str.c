/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_word_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:42:10 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/30 16:22:00 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "expander.h"
#include "xlibc.h"

static bool	is_valid_first_char_after_dollar(char c)
{
	return (ft_isalpha(c) || c == '_' || c == '?' || c == '$');
}

static char	*find_dollar(char **cur_ptr)
{
	char	*dollar_ptr;

	dollar_ptr = ft_strchr(*cur_ptr, '$');
	if (!dollar_ptr || *(dollar_ptr + 1) == '\0')
	{
		*cur_ptr = NULL;
		return (NULL);
	}
	if (!is_valid_first_char_after_dollar(*(dollar_ptr + 1)))
	{
		*cur_ptr = dollar_ptr + 1;
		return (NULL);
	}
	if (*(dollar_ptr + 1) == '$')
	{
		*cur_ptr = dollar_ptr + 2;
		return (NULL);
	}
	return (dollar_ptr);
}

static void	replace_word_str(char **word, char *expanded_value, char *suffix)
{
	size_t	len;
	char	*new_word;

	len = ft_strlen(*word) + ft_strlen(expanded_value) + ft_strlen(suffix);
	new_word = xmalloc(len + 1);
	ft_strcpy(new_word, *word);
	ft_strlcat(new_word, expanded_value, len + 1);
	ft_strlcat(new_word, suffix, len + 1);
	free(*word);
	*word = new_word;
}

static void	expand_variable(char **word, char **cur_ptr, t_mgr *mgr)
{
	char	*dollar_ptr;
	char	*suffix;
	char	*expanded_value;
	size_t	updated_len;
	bool	should_free;

	dollar_ptr = find_dollar(cur_ptr);
	if (!dollar_ptr)
		return ;
	should_free = *(dollar_ptr + 1) == '?';
	expanded_value = get_expanded_value(dollar_ptr, &suffix, mgr);
	if (!expanded_value)
		return ;
	*dollar_ptr = '\0';
	updated_len = ft_strlen(*word) + ft_strlen(expanded_value);
	replace_word_str(word, expanded_value, suffix);
	if (should_free)
		free(expanded_value);
	*cur_ptr = *word + updated_len;
}

void	expand_word_str(char **word, t_mgr *mgr)
{
	char	*current_ptr;

	if (!word || !*word)
		return ;
	current_ptr = *word;
	while (current_ptr && *current_ptr)
	{
		expand_variable(word, &current_ptr, mgr);
	}
}
