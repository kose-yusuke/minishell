/* error.c - エラー処理に関する関数の定義。 */
#include "minishell.h"
#include "error.h"

// TODO: デバッグ用の関数
void	assert_error(const char *msg, char *location)
{
	write(STDERR_FILENO, "Assert Error: ", 14);
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	if (location)
	{
		write(STDERR_FILENO, " near ", 6);
		write(STDERR_FILENO, location, ft_strlen(location));
	}
	write(STDERR_FILENO, "\n", 1);
	exit(255);
}

void	error_exit(const char *msg, int exit_status)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(exit_status);
}

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
	write(STDERR_FILENO, "minishell: syntax error near unexpected token `", 47);
	if (unexpected_token)
	{
		const char *token_str = get_token_string(unexpected_token);
		write(STDERR_FILENO, token_str, ft_strlen(token_str));
	}
	write(STDERR_FILENO, "'\n", 2);
}

void	report_error(char *cmd, char *file, char *info)
{
	write(STDERR_FILENO, "minishell:", 10);
	if (cmd)
	{
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, ":", 1);
	}
	if (file)
	{
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, file, ft_strlen(file));
		write(STDERR_FILENO, ":", 1);
	}
	if (info)
	{
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, info, ft_strlen(info));
	}
	write(STDERR_FILENO, "\n", 1);
}
