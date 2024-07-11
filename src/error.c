/* error.c -- Functions for handling errors. */
#include "minishell.h"

// TODO: おそらくdebug用の関数
void	assert_error(const char *msg, char *location)
{
	dprintf(STDERR_FILENO, "Assert Error: %s near %s\n", msg, location);
	exit(255);
}

// TODO: おそらくdebug用の関数
void	todo(const char *msg)
{
	dprintf(STDERR_FILENO, "TODO: %s\n", msg);
	exit(255);
}

void	error_exit(const char *msg, int exit_status)
{
	dprintf(STDERR_FILENO, "minishell: %s\n", msg);
	exit(exit_status);
}

void	lexer_error(const char *location, char **rest, char *line)
{
	dprintf(STDERR_FILENO, "minishell: syntax error near %s\n", location);
	while (*line)
		line++;
	*rest = line;
}

void	report_error(char *cmd, char *file, char *info)
{
	fprintf(stderr, "minishell:");
	if (cmd)
	{
		fprintf(stderr, " %s:", cmd);
	}
	if (file)
	{
		fprintf(stderr, " %s:", file);
	}
	if (info)
	{
		fprintf(stderr, " %s", info);
	}
	fprintf(stderr, "\n");
}
