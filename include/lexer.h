/* lexer.h - 入力をトークンに分割する字句解析器の定義。 */
#ifndef LEXER_H
# define LEXER_H

# include <stdbool.h> // bool
# include <stdlib.h>  // malloc, free
# include <string.h>  // strchr, strncmp, strudup, strlen

typedef enum e_token_type
{
	TK_UNDEF_TOKEN = -1, // 未定義のトークンは通常存在しない値を使う
	TK_PARSE_ERROR = -2, // 構文エラー
	TK_EOF = '\0',       // EOF

	// word token
	TK_WORD = 'a',         // WORD 一般的な単語や文字列
	TK_SQUOTED_STR = '\'', // シングルクオート文字列
	TK_DQUOTED_STR = '\"', // ダブルクオート文字列

	// meta and op token
	TK_PIPE = '|',         // パイプ
	TK_REDIR_IN = '<',     // リダイレクト入力
	TK_REDIR_OUT = '>',    // リダイレクト出力
	TK_HEREDOC = 'H',      // ヒアドキュメント、大文字で他と区別
	TK_REDIR_APPEND = 'A', // リダイレクト追加、大文字で他と区別
	TK_WHITESPACE = ' ',   // 空白
	TK_TAB = '\t',         // タブ
	TK_NEWLINE = '\n'      // 改行
}						t_token_type;

typedef struct s_token
{
	enum e_token_type	type;
	char				*word;
	char				*end;
	struct s_token		*next;
}						t_token;

t_token					*lexer(char *s);

#endif
