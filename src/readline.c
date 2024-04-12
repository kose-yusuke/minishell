/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 18:23:40 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/04/12 05:12:40 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "parser.h"

// デバッグ用
int	print_list(t_token *tok)
{
	int	i;

	i = 2;
	while (i > 0)
	{
		printf("%s\n", tok->word);
		printf("%u\n", tok->kind);
		tok = tok->next;
		i--;
	}
	return (1);
}

void	interpret(char *line, t_mgr *mgr)
{
	char	**argv;
	t_token	*tok;
	extern char	**environ;

	tok = NULL; // FIXME: mallocすべき
	mgr->tok = tokenize(line);
	mgr->ast = parse_token(mgr);
	// print_list(tok);
	if (tok->kind == TK_EOF)
		;
	else if (mgr->syntax_error)
		mgr->status = ERROR_TOKENIZE;
	else
	{
		expand(tok);
		argv = token_list_to_argv(tok);
		mgr->status = exec(argv);
		free_argv(argv);
	}
	free_tok(tok);
}

// FIXME: char **envpを使っていないので必要か確認してから修正する
int	ft_readline(char **envp, t_mgr *mgr)
{
	char	*line;
	// pid_t pid; TODO: fork()を使わなくてもいいのかな？

	rl_outstream = stderr;
	mgr->status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		// TODO:　chdirコマンドの処理は、親プロセスで行う必要がある（未実装）
		// TODO: intepret line as a command
		interpret(line, mgr);
		// pid = fork1();
		// if (pid == 0)
		// 	runcmd(parsecmd(line));
		// wait(pid);
		free(line);
	}
	exit(mgr->status);
}

// FIXME: minishellのコマンド単位でforkするタイミングなんだけど、lexer, parserの時点ではforkせずに、execに入る直前に　forkするべき？

