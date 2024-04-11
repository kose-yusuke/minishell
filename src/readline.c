/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 18:23:40 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/04/12 02:10:49 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "parser.h"

// デバッグ用
int	print_list(t_token *tok)
{
    int i = 2;
	while (i > 0)
	{
		printf("%s\n", tok->word);
        printf("%u\n", tok->kind);
		tok = tok->next;
        i--;
	}
	return (1);
}

void interpret(char *line, t_mgr *mgr)
{
    char		**argv;
    t_token *tok;

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

int	ft_readline(char **envp, t_mgr *mgr)
{
	char	*line;
    int status;

	rl_outstream = stderr;
    mgr->status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		// TODO: intepret line as a command
        interpret(line, mgr);
		free(line);
	}
	exit(mgr->status);
}
