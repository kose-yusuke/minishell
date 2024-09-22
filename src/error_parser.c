/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:14:13 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 01:15:04 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "minishell.h"

static const char	*get_token_string(t_token *token)
{
	if (token->type == TK_EOF)
		return ("newline");
	if ((token->type == TK_WORD || token->type == TK_SQUOTE
			|| token->type == TK_DQUOTE) && token->word)
		return ((const char *)(token->word));
	if (token->type == TK_IO_NUM)
		return ("IO number");
	if (token->type == TK_PIPE)
		return ("|");
	if (token->type == TK_REDIR_IN)
		return ("<");
	if (token->type == TK_REDIR_OUT)
		return (">");
	if (token->type == TK_HEREDOC)
		return ("<<");
	if (token->type == TK_APPEND)
		return (">>");
	if (token->type == TK_BLANK)
		return ("blank");
	return ("unknown or undefined token");
}

void	parser_error(t_token *unexpected_token)
{
	const char	*token_str = get_token_string(unexpected_token);

	write(STDERR_FILENO, "minishell: syntax error near unexpected token `", 47);
	if (unexpected_token)
	{
		write(STDERR_FILENO, token_str, ft_strlen(token_str));
	}
	write(STDERR_FILENO, "'\n", 2);
}
