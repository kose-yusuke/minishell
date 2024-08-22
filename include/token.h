/* token.h - トークンの型定義 */
#ifndef TOKEN_H
# define TOKEN_H

# include <stdbool.h>
# include <stdlib.h>

typedef enum e_token_type
{
	TK_UNDEF_TOKEN = -1, // 未定義のトークン
	TK_PARSE_ERROR = -2, // 構文エラー
	TK_EOF = 0,          // ファイルの終わり

	// Word tokens
	TK_WORD,   // 一般的な単語や文字列
	TK_SQUOTE, // シングルクオートで囲まれた文字列
	TK_DQUOTE, // ダブルクオートで囲まれた文字列
	TK_IO_NUM, // IO番号

	// Meta and operator tokens
	TK_PIPE,      // パイプ '|'
	TK_REDIR_IN,  // リダイレクト入力 '<'
	TK_REDIR_OUT, // リダイレクト出力 '>'
	TK_HEREDOC,   // ヒアドキュメント '<<'
	TK_APPEND,    // リダイレクト追加 '>>'
	TK_BLANK,     // 空白 ' ' または '\t'
}						t_token_type;

typedef struct s_token
{
	enum e_token_type	type;
	char				*word;
	struct s_token		*next;
}						t_token;

/* token_checks.c */
bool					is_word_token(t_token *token);
bool					is_quoted_token(t_token *token);
bool					is_word_or_quoted_token(t_token *token);
bool					is_io_num_token(t_token *token);
bool					is_redir_token(t_token *token);

/* token_operations.c  */
void					advance(t_token **token);
void					skip_blanks(t_token **token);
bool					peek(t_token **token, t_token_type type);
bool					consume(t_token **token, t_token_type type);
t_token					*next_token(t_token **token);

t_token					*new_token(t_token_type type, char **q, char **eq);

#endif
