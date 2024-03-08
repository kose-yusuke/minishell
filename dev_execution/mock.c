/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:30:23 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/08 16:49:03 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static const char	whitespace[] = " \t\r\n\v";
static const char	symbols[] = "<|>";

/**
 *  xv6を参考にした、executionのユニットテスト作成を目的とするmockのコード
 * */

pid_t	fork1(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("fork");
	return (pid);
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && strchr(whitespace, *s))
		s++;
	*ps = s;
	return (*s && strchr(toks, *s));
}

// lexer

/*
'"'と'\''については、他の部分の実装が固まってから後で着手

- parserに進む前にtokenの時点で"や'で囲まれた部分を文字列として扱うための処理
- quoteが囲まれていない時にエラーとして弾く（minishellの仕様）
- 囲んだquoteの種類によって環境変数の展開が発生するかどうかを考慮しないといけない

環境変数の展開は、実行の直前に文字列を確認して行う

文字列を 'a' トークンとして処理し、環境変数の展開が許されないケースを 'b' トークンとして区別することを予定している
 */

static const char	whitespace[] = " \t\r\n\v";
static const char	symbols[] = "<|>";

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	s = *ps;
	while (s < es && strchr(whitespace, *s))
		s++;
	// トークンの開始地点を指すポインタを更新
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
			// heredocの実装のやり方がよくわからない。
			//とりあえず '-' tokenを返す
			ret = '-';
			s++;
		}
		break ;
	case '>':
		s++;
		if (*s == '>')
		{
			ret = '+';
			s++;
		}
		break ;
	case '"':
		// 文字列の処理はやり方がわからないため一旦省略（こちらは後の環境変数の展開なども含むため'\''とはなんらかの形で区別する必要がある）
	case '\'':
		// 文字列の処理はやり方がわからないため一旦省略
	default:
		ret = 'a';
		while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
			s++;
		break ;
	}
	// トークンの終了地点を指すポインタを更新
	if (eq)
		*eq = s;
	while (s < es && strchr(whitespace, *s))
		s++;
	// 次のトークンの開始地点を指すポインタを更新
	*ps = s;
	return (ret);
}

// parser
struct cmd			*parseline(char **, char *);
struct cmd			*parsepipe(char **, char *);
struct cmd			*parseexec(char **, char *);
struct cmd			*nulterminate(struct cmd *);

// constructor
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

struct cmd	*parseredirs(struct cmd *cmd, char **ps, char *es)
{
	int	tok;

	char *q, *eq;
	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a')
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
	// `(`の処理はいらないため省略
	// 手動ポリモーフィズム
	// execcmdをcmd型にキャストしたものをexeccmdとして再キャストする
	ret = execcmd();
	cmd = (struct execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|)&;"))
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
