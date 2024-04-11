/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev_header.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:51:35 by sakitaha          #+#    #+#             */
/*   Updated: 2024/04/11 01:39:32 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEV_HEADER_H
# define DEV_HEADER_H

# include "libft.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAXARGS 10

// 現在のtoken種類（minishell）
typedef enum e_token_kind
{
	TK_WORD,
	TK_OP,
	TK_EOF
}				t_token_kind;

/*

あなたのMinishellプロジェクトのための`tokentype`定義は、課題文を基にすると適切なスタートポイントになっています。課題文で要求されている機能を踏まえて、トークンの追加や削除を検討する価値があるでしょう。

課題文に基づいて特に注目すべき点は以下の通りです：

1. **リダイレクションとパイプ**:
   - `<`, `>`, `<<`, `>>`,
	`|` に関しては、あなたの現在の定義でカバーされています。`>&` や `<&` は課題文には含まれていないため、対応する必要はありません。

2. **クォート処理**:

	- 課題文ではシングルクォート(`'`)とダブルクォート(`"`)の扱いが明確にされています。これらは特殊文字の解釈を制御します。`TK_WORD`はこれらのクォートに囲まれた文字列を適切に扱う必要がありますが、クォート自体をトークンとして扱うかどうかは、実装の詳細によります。

3. **環境変数と特殊文字**:

	- `$` による環境変数の展開と、`$?`による直前のコマンドの終了ステータスへのアクセスは必要です。これらを扱うためのトークンが必要になるかもしれません。

4. **ビルトインコマンド**:
   - `echo`, `cd`, `pwd`, `export`, `unset`, `env`,
	`exit` などのビルトインコマンドの処理には、直接トークンタイプを設定する必要はありませんが、これらのコマンドを識別し、適切に処理できる必要があります。

5. **制御構造**:
   - 課題のボーナスパートでは、`&&`, `||`, `*`（ワイルドカード）が含まれますが、これはボーナスとして扱われ、基本パートでは不要です。

結論として、あなたのトークン定義はMinishellプロジェクトの基本要件を満たすための良い基盤を提供しています。しかし、パースする際の詳細や特定の機能の実装に応じて、必要に応じてトークンを微調整することが重要です。特に、環境変数の扱いや特殊トークンの処理には注意を払い、プロジェクトの目標に合わせた適切なトークンセットを確立してください。

2. **特殊トークン**:

	- `TK_NUMBER`トークンは、特に算術演算をサポートしない限り、Minishellで必要ないかもしれません。入力としての数値は通常、引数やオプションの値として扱われ、特別なトークンタイプを必要としません。
   - `TK_ERROR`, `TK_UNDEF` などのエラー処理に関するトークンは、エラーハンドリング戦略に応じて適切に使用するか決定します。

3. **引用符と空白の扱い**:
   - `CHAR_QOUTE`, `CHAR_DQUOTE`,
	`CHAR_WHITESPACE` などのトークンは、文字列の解析や、引用符で囲まれたテキストブロックの処理に重要です。特に引用符の内外での空白の扱いは、シェルスクリプトの挙動において基本的な部分を形成します。

4. **`extern char **environ;
` の使用 ** :
	- `environ`へのアクセスに関連するトークンは不要ですが、環境変数の処理に関わるロジックをLexerやParserでどう扱うかを考慮する必要があります。

				## #提案 -
				Minishellの要件と目的に基づいて、特定のトークンを追加または削除することを検討してください。例えば、より複雑なリダイレクションやパイプラインのサポート、引用符によるグルーピングやエスケープ処理などです。
					-
				MinishellがBashのどの機能を模倣し、どの機能を省略するかによって、必要なトークンタイプが変わります。プロジェクトのスコープを明確に定義し、それに基づいてLexerの設計を調整してください。

				あなたのLexer設計は良い出発点ですが、プロジェクトの要件に応じて適宜調整を行うことが重要です。さらに複雑なシェルの機能をサポートする場合は、より多くのトークンタイプが必要になるかもしれません。反対に、Minishellの機能をシンプルに保つ場合は、現在のトークンセットで十分かもしれません。
 */

// bashのtokenを参考に、不要なものを削除してminishellのtokenを定義しようとしている
// 以下は不要なtoken（直接charを代入すれば良いため）
// TK_NEWLINE = 52, /* '\n'  */
// TK_PIPE = 54,    /* '|'  */
// TK_GREATER = 55, /* '>'  */
// TK_LESS = 56,    /* '<'  */
typedef enum e_mytokentype
{
	TK_EMPTY = -2, /* 未使用のtoken */
	TK_EOF = 0,
	/* "end of file" ファイルの終わり、入力の終わり */
	TK_PARSE_ERROR = 256,     /* error 構文エラー */
	TK_UNDEF_TOKEN = 257,     /* "invalid token" 未定義、または無効なtoken */
	TK_WORD = 281,            /* WORD 一般的な単語や文字列 */
	TK_ASSIGNMENT_WORD = 282, /* ASSIGNMENT_WORD 代入を含む単語。例えば`VAR=value`形式 */
	TK_REDIR_WORD = 283,      /* REDIR_WORD リダイレクションに関連する単語 */
	TK_GREATER_GREATER = 290,
	/* GREATER_GREATER `>>` 出力の追記リダイレクト */
	TK_LESS_LESS = 291, /* LESS_LESS `<<` ヒアドキュメント */

}				t_mytokentype;

typedef enum e_cmd_type
{
	NONE,
	EXEC,
	REDIR,
	PIPE
}				t_cmd_type;

typedef struct s_cmd
{
	t_cmd_type	type;
}				t_cmd;

typedef struct s_execcmd
{
	t_cmd_type	type;
	char		*argv[MAXARGS];
	char		*eargv[MAXARGS];
}				t_execcmd;

typedef struct s_redircmd
{
	t_cmd_type	type;
	t_cmd		*cmd;
	char		*filepath;
	char		*efilepath;
	int oflag;     // O_WRONLY | O_CREATE, O_RDONLY とかを入れる 元々mode
	int fd;        // STDOUT_FILENO, STDIN_FILENO とかを入れる
	bool here_doc; // ここ? here_docはexpansionの時に展開する

}				t_redircmd;

typedef struct s_pipecmd
{
	t_cmd_type	type;
	t_cmd		*left;
	t_cmd		*right;
}				t_pipecmd;

void			error_exit(char *err_msg);
void			perror_exit(char *err_msg);

#endif
