/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:48:26 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/09/01 04:12:50 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd.h"
#include "error.h"
#include "free.h"
#include "xlibc.h"

void	print_argv(char **argv)
{
	int	i;

	i = 0;
	if (argv == NULL)
	{
		printf("argv is NULL\n");
		return ;
	}
	printf("argv:\n");
	while (argv[i])
	{
		if (argv[i] == NULL)
		{
			printf("argv[%d] is NULL\n", i);
		}
		else
		{
			printf("argv[%d]: %s\n", i, argv[i]);
		}
		i++;
	}
}

char	*ft_strdup(char *src)
{
	char	*p;
	long	len;

	if (src == NULL)
		return (NULL);
	len = ft_strlen(src);
	p = (char *)xmalloc(sizeof(char) * (len + 1));
	if (!p)
		return (0);
	ft_strcpy(p, src);
	return (p);
}

char	*ft_strndup(char *src, long len)
{
	char	*p;

	if (src == NULL)
		return (NULL);
	p = (char *)xmalloc(sizeof(char) * (len + 1));
	if (!p)
		return (NULL);
	ft_strncpy(p, src, len);
	p[len] = '\0'; // 文字列終端を追加
	return (p);
}

char	**convert_list_to_array(t_word *word_list)
{
	char	**argv;
	t_word	*tmp_word;
	size_t	len;
	size_t	i;

	if (!word_list || !word_list->token || !word_list->token->word)
	{
		return (NULL);
	}
	len = 0;
	tmp_word = word_list;
	while (tmp_word)
	{
		tmp_word = tmp_word->next;
		len++;
	}
	argv = (char **)xmalloc(sizeof(char *) * (len + 1));
	tmp_word = word_list;
	i = 0;
	while (tmp_word)
	{
		if (!is_word_or_quoted_token(tmp_word->token))
			break ;
		// XXX: 提案 argvはtoken->wordを指すだけでもいいかも？
		// argv[i] = tmp_word->token->word;
		argv[i] = ft_strdup(tmp_word->token->word);
		// ft_strdupの中でexitしているので、以下はコメントアウト
		// if (!argv[i])
		// {
		// 	// while (i > 0)
		// 	// 	free(argv[--i]);
		// 	// free(argv);
		// 	free_argv(argv);
		// 	perror("strdup");
		// }
		i++;
		tmp_word = tmp_word->next;
	}
	argv[i] = NULL;
	return (argv);
}

// char	**convert_list_to_array(t_execcmd *ecmd)
// {
// 	char	**argv;
// 	t_token	*tmp_token;
// 	size_t	len;
// 	size_t	i;

// 	print_wordlist(ecmd->word_list);
// 	len = 0;
// 	tmp_token = ecmd->word_list->token;
// 	while (tmp_token)
// 	{
// 		tmp_token = tmp_token->next;
// 		len++;
// 	}
// 	if (!(argv = (char **)malloc(sizeof(char *) * (len + 1))))
// 		error_exit("usage: ./minishell", EXIT_FAILURE);
// 	tmp_token = ecmd->word_list->token;
// 	i = 0;
// 	while (tmp_token->type != TK_EOF)
// 	{
//         if (tmp_token->type == TK_WORD || tmp_token->type == TK_DQUOTE
//	|| tmp_token->type == TK_SQUOTE)
// 		{
// 			argv[i] = ft_strdup(tmp_token->word);
// 			i++;
// 		}
// 		tmp_token = tmp_token->next;
// 		// if (!argv[i])
// 			// error_exit("usage: ./minishell", EXIT_FAILURE);
// 			// break ;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }

// できればreturn valueをt_statusにかえたい
int	exec_builtin(char **argv, t_mgr *mgr)
{
	int	status;

	// print_argv(argv);
	if (strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv);
	else if (strcmp(argv[0], "export") == 0)
		status = builtin_export(argv, mgr);
	else if (strcmp(argv[0], "unset") == 0)
		status = builtin_unset(argv, mgr);
	else if (strcmp(argv[0], "env") == 0)
		status = builtin_env(argv, mgr, 1);
	else if (strcmp(argv[0], "cd") == 0)
		status = builtin_cd(argv, mgr);
	else if (strcmp(argv[0], "echo") == 0)
		status = builtin_echo(argv);
	else if (strcmp(argv[0], "pwd") == 0)
		status = builtin_pwd(argv);
	return (status);
}

bool	is_builtin(char *cmd_name)
{
	const char		*builtin_commands[] = {"exit", "export", "unset", "env",
				"cd", "echo", "pwd"};
	unsigned int	i;

	if (!cmd_name)
		return (false);
	i = 0;
	while (i < sizeof(builtin_commands) / sizeof(*builtin_commands))
	{
		if (strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}
