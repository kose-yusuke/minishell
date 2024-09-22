/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 01:01:05 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 01:09:55 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// /* debug_helpers.c - デバッグ用の関数 */

// #include "debug_helpers.h"
// #include <stdio.h>
// #include <unistd.h>

// #ifdef DEBUG_MODE

// static const char	*token_to_str(t_token_type type)
// {
// 	switch (type)
// 	{
// 	case TK_UNDEF_TOKEN:
// 		return ("TK_UNDEF_TOKEN");
// 	case TK_PARSE_ERROR:
// 		return ("TK_PARSE_ERROR");
// 	case TK_EOF:
// 		return ("TK_EOF");
// 	case TK_WORD:
// 		return ("TK_WORD");
// 	case TK_SQUOTE:
// 		return ("TK_SQUOTE");
// 	case TK_DQUOTE:
// 		return ("TK_DQUOTE");
// 	case TK_IO_NUM:
// 		return ("TK_IO_NUM");
// 	case TK_PIPE:
// 		return ("TK_PIPE");
// 	case TK_REDIR_IN:
// 		return ("TK_REDIR_IN");
// 	case TK_REDIR_OUT:
// 		return ("TK_REDIR_OUT");
// 	case TK_HEREDOC:
// 		return ("TK_HEREDOC");
// 	case TK_APPEND:
// 		return ("TK_APPEND");
// 	case TK_BLANK:
// 		return ("TK_BLANK");
// 	default:
// 		return ("UNKNOWN_TYPE");
// 	}
// }

// void	print_tokens(t_token *token)
// {
// 	t_token	*current;
// 	size_t	i;

// 	current = token;
// 	i = 0;
// 	while (current)
// 	{
// 		printf("--------------------------------\n");
// 		printf("Token [%zu]\n", i++);
// 		printf("Type: %s\n", token_to_str(current->type));
// 		if (current->word)
// 		{
// 			printf("Word: %s\n", current->word);
// 		}
// 		current = current->next;
// 	}
// }

// void	print_word_list(t_word *word_list)
// {
// 	t_word	*current;

// 	current = word_list;
// 	while (current)
// 	{
// 		printf("[%s](%s) -> ", current->token->word,
// 			token_to_str(current->token->type));
// 		current = current->next;
// 	}
// 	printf("\n");
// }

// void	print_redir_list(t_redir *redir_list)
// {
// 	t_redir	*current;

// 	current = redir_list;
// 	while (current)
// 	{
// 		printf("Redir: %s, FD: %d\n", token_to_str(current->redir_type),
// 			current->fd);
// 		printf("Redir word :");
// 		print_word_list(current->word_list);
// 		current = current->next;
// 	}
// }

// void	print_cmd(t_cmd *cmd)
// {
// 	t_execcmd	*ecmd;
// 	t_pipecmd	*pcmd;

// 	if (!cmd)
// 	{
// 		printf("No command\n");
// 		return ;
// 	}
// 	if (cmd->type == EXEC)
// 	{
// 		ecmd = (t_execcmd *)cmd;
// 		printf("EXEC command:\n");
// 		printf("Word list: ");
// 		print_word_list(ecmd->word_list);
// 		print_redir_list(ecmd->redir_list);
// 	}
// 	else if (cmd->type == PIPE)
// 	{
// 		pcmd = (t_pipecmd *)cmd;
// 		printf("PIPE command:\n");
// 		printf("Left:\n");
// 		print_cmd(pcmd->left);
// 		printf("Right:\n");
// 		print_cmd(pcmd->right);
// 	}
// 	else
// 	{
// 		printf("Unknown command type\n");
// 	}
// }

// #endif // DEBUG_MODE
/*
void	print_status(int status, char *location)
{
	char	*g_status_str;
	char	*mgr_status_str;

	if (1)
		return ;
	if (g_status != 0 || status != 0)
	{
		write(1, "\n\n", 2);
		write(1, "<<", 2);
		write(1, location, ft_strlen(location));
		write(1, ">>", 2);
		g_status_str = ft_itoa(g_status);
		mgr_status_str = ft_itoa(status);
		write(1, " g_status : ", 12);
		write(1, g_status_str, ft_strlen(g_status_str));
		write(1, ", mgr_status : ", 15);
		write(1, mgr_status_str, ft_strlen(mgr_status_str));
		write(1, "\n\n", 2);
		free(mgr_status_str);
		free(g_status_str);
	}
}

#include <unistd.h> // for getpid

void	check_for_leaks(void)
{
	pid_t	pid;
	char	command[256];

	pid = getpid();
	snprintf(command, sizeof(command), "leaks -q %d", pid);
	system(command);
}

// check_for_leaks();
		// system("leaks -q minishell"); // TODO: remove

 */
