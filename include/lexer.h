#ifndef LEXER_H
# define LEXER_H

# include <stdbool.h>

typedef enum e_token_type
{
	TK_UNDEF_TOKEN = -1, // 未定義のトークン
	TK_PARSE_ERROR = -2, // 構文エラー
	TK_EOF = 0,          // ファイルの終わり

	// Word tokens
	TK_WORD,        // 一般的な単語や文字列
	TK_SQUOTED_STR, // シングルクオートで囲まれた文字列
	TK_DQUOTED_STR, // ダブルクオートで囲まれた文字列
	TK_IO_NUMBER,   // IO番号

	// Meta and operator tokens
	TK_PIPE,         // パイプ '|'
	TK_REDIR_IN,     // リダイレクト入力 '<'
	TK_REDIR_OUT,    // リダイレクト出力 '>'
	TK_HEREDOC,      // ヒアドキュメント '<<'
	TK_REDIR_APPEND, // リダイレクト追加 '>>'
	TK_WHITESPACE,   // 空白 ' '
	TK_TAB,          // タブ '\t'
	TK_NEWLINE       // 改行 '\n'
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
