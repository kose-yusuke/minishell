/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:55:19 by koseki.yusu       #+#    #+#             */
/*   Updated: 2024/04/12 05:13:08 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"
#include "../include/minishell.h"

t_token *new_token(char *str ,t_token_kind kind)
{
	t_token *tmp;

	tmp = calloc(1,sizeof(t_token));
	if (!tmp)
		fatal_error("calloc");
	tmp->word = str;
	tmp->kind = kind;
	return (tmp);
}

bool is_operator(const char *line)
{
	static char *const operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t i;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(line, operators[i]))
			return (true);
		i++;
	}
	return (false);
}

bool is_metacharacter(char c)
{
	return (c && strchr("|&;()<> \t\n", c));
}

bool is_word(const char *line)
{
	return (*line && !is_metacharacter(*line));
}

t_token *operator(char **rest, char *line)
{
	static char	*const	operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
	size_t				i = 0;
	char				*op;

	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (startswith(line, operators[i]))
		{
			op = strdup(operators[i]);
			if (!op)
				fatal_error("strdup");
			*rest = line + strlen(op);
			return (new_token(op,TK_OP));
		}
		i++;
	}
	assert_error("Unexpected operator");
	return (NULL);
}

t_token *word(char **rest, char *line)
{
	const char *start = line;
	char *word;

	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR)
		{
			line++;
			while (*line != SINGLE_QUOTE_CHAR)
			{
				if (*line == '\0')
					todo("Unclosed single quote");
				line++;
			}
			line++;
		}
		else if (*line == DOUBLE_QUOTE_CHAR)
		{
			line++;
			while (*line != DOUBLE_QUOTE_CHAR)
			{
				if (*line == '\0')
					todo("Unclosed double quote");
				line++;
			}
			line++;
		}
		else
			line++;
	}
	word = strndup(start,line - start);
	if (word == NULL)
		fatal_error("strndup");
	*rest = line;
	return (new_token(word, TK_WORD));

}

t_token	*tokenize(char *line)
{
	t_token	head;
	t_token	*tok;

	head.next = NULL;
	tok = &head;
	while (*line)
	{
		if (consume_blank(&line, line))
			continue ;
		else if (is_operator(line))
			tok = tok->next = operator(&line, line);
		else if (is_word(line))
			tok = tok->next = word(&line, line);
		else
			assert_error("Unexpected Token");
	}
	tok->next = new_token(NULL, TK_EOF);
	return (head.next);
}

char	**tail_recursive(t_token *tok, int nargs, char **argv)
{
	if (tok == NULL || tok->kind == TK_EOF)
		return (argv);
	argv = reallocf(argv, (nargs + 2) * sizeof(char *));
	argv[nargs] = strdup(tok->word);
	if (argv[nargs] == NULL)
		fatal_error("strdup");
	argv[nargs + 1] = NULL;
	return (tail_recursive(tok->next, nargs + 1, argv));
}


char	**token_list_to_argv(t_token *tok)
{
	char	**argv;

	argv = calloc(1, sizeof(char *));
	if (argv == NULL)
		fatal_error("calloc");
	return (tail_recursive(tok, 0, argv));
}
