/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:30:23 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/09 21:03:14 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/**
 * This file is inspired by xv6's sh.c, adapted for unit testing
 * of execution components for a 42 minishell project.
 * Original xv6 code: https://github.com/mit-pdos/xv6-public
 * on Unix Version 6 re-implementation using ANSI C.
 * xv6 Copyright 2006-2018 by Frans Kaashoek, Robert Morris, and Russ Cox.
 *
 * 42のminishellプロジェクト、execution部分のユニットテストを目的としたコードです。
 * xv6のsh.cに基づいており、minishellの要件に合わせて一部改変しています。
 * Gitリポジトリ（xv6）: https://github.com/mit-pdos/xv6-public
 * xv6の著作権は2006-2018年、Frans Kaashoek, Robert Morris, Russ Coxに帰属します。
 */

static const char	whitespace[] = " \t\r\n\v";
static const char	symbols[] = "<|>";

/**
 * forkの失敗時にはエラーMSGと共に終了する
 */
pid_t	fork1(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("fork");
	return (pid);
}

/**
 * 空白文字" \t\r\n\v"を飛ばして、**psの位置を更新する
 * 先頭の文字が指定された文字列に含まれるかどうかを返す
 */
int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && strchr(whitespace, *s))
		s++;
	*ps = s;
	return (*s && strchr(toks, *s));
}

/**
 * lexer (gettoken)
 * 通常の文字列 -> 'a' token
 * 環境変数を展開しない文字列 ->'b' token
 */
int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	s = *ps;
	while (s < es && strchr(whitespace, *s))
		s++;
	// tokenの開始地点を指すポインタを更新
	if (q)
		*q = s;
	ret = *s;
	switch (*s)
	{
	case 0:
		break ;
	case '|':
		s++;
		break ;
	case '<':
		s++;
		if (*s == '<')
		{
			ret = '-'; // token for `<<`
			s++;
		}
		break ;
	case '>':
		s++;
		if (*s == '>')
		{
			ret = '+'; // token for `>>`
			s++;
		}
		break ;
	case '"':
		ret = 'a'; // 通常の文字列token
		s++;       // 最初のdouble quote
		while (s < es && *s != '"')
			s++; // quote内の文字を飛ばす
		if (s < es)
			s++; // 終了のdouble quote
		else
			error_exit("unclosed double quote");
		break ;
	case '\'':
		ret = 'b'; // 環境変数を展開しない文字列token
		s++;       // 最初のsingle quote
		while (s < es && *s != '\'')
			s++;
		if (s < es)
			s++; // 終了のsingle quote
		else
			error_exit("unclosed single quote");
		break ;
	default:
		ret = 'a';
		while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
			s++;
		break ;
	}
	// tokenの終了地点を指すポインタを更新
	if (eq)
		*eq = s;
	while (s < es && strchr(whitespace, *s))
		s++;
	// 次のtokenの開始地点までポインタを更新
	*ps = s;
	return (ret);
}

// parser
struct cmd			*parseline(char **, char *);
struct cmd			*parsepipe(char **, char *);
struct cmd			*parseexec(char **, char *);
struct cmd			*nulterminate(struct cmd *);

/**
 * execcmd, redircmd, pipecmdのconstructor
 * struct cmdを基底クラスとして、castした構造体を返す
 */
struct cmd	*execcmd(void)
{
	struct execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct cmd *)cmd);
}

struct cmd	*redircmd(struct cmd *subcmd, char *file, char *efile, int mode,
		int fd)
{
	struct redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = fd;
	return ((struct cmd *)cmd);
}

struct cmd	*pipecmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct cmd *)cmd);
}

struct cmd	*parsecmd(char *s)
{
	char		*es;
	struct cmd	*cmd;

	es = s + strlen(s);
	cmd = parseline(&s, es);
	peek(&s, es, "");
	if (s != es)
	{
		printf(2, "leftovers: %s\n", s);
		error_exit("syntax error parsecmd");
	}
	nulterminate(cmd);
	return (cmd);
}

struct cmd	*parseline(char **ps, char *es)
{
	struct cmd	*cmd;

	cmd = parsepipe(ps, es);
	// &と;の処理はいらないため省略
	return (cmd);
}

struct cmd	*parsepipe(char **ps, char *es)
{
	struct cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

// 重複するredirectionが複数あるケースなどに対応していない気がします
struct cmd	*parseredirs(struct cmd *cmd, char **ps, char *es)
{
	int	tok;

	char *q, *eq;
	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a') // 次のtokenを確認
			panic("missing file for redirection");
		switch (tok)
		{
		case '<':
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
			break ;
		case '>':
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT, 1);
			break ;
		case '+': // >>
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT, 1);
			break ;
		}
	}
	return (cmd);
}

struct cmd	*parseexec(char **ps, char *es)
{
	struct execcmd	*cmd;
	struct cmd		*ret;

	char *q, *eq;
	int tok, argc;
	ret = execcmd();             // コンストラクタ
	cmd = (struct execcmd *)ret; // 手動ポリモーフィズム
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|")) // 元々"|)&;"
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
			error_exit("syntax");
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		argc++;
		if (argc >= MAXARGS)
			error_exit("too many args");
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (ret);
}

int	main(void)
{
	char	*line;
	pid_t	pid;

	// readlineの出力ストリームをstderrに設定
	rl_outstream = stderr;
	while (true)
	{
		line = readline("mock$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		// TODO:　chdirコマンドの処理は、親プロセスで行う必要がある（未実装）
		// TODO: intepret line as a command
		pid = fork1();
		// 子プロセスでコマンドを実行
		if (pid == 0)
			runcmd(parsecmd(line));
		// 親プロセスは子プロセスの終了を待つ
		wait(NULL);
		free(line);
	}
	exit(0);
}
