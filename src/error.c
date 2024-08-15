/* error.c - エラー処理に関する関数の定義。 */
#include "minishell.h"

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

void	parser_error(const char *unexpected_token)
{
	write(STDERR_FILENO, "minishell: syntax error near unexpected token '", 44);
	if (unexpected_token)
		write(STDERR_FILENO, unexpected_token, ft_strlen(unexpected_token));
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
