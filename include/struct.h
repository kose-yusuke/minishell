#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_mgr
{
	int					status;
	bool				syntax_error;
}						t_mgr;

typedef enum e_token_type
{
	TK_UNDEF_TOKEN, /* 未定義のトークン */
	TK_PARSE_ERROR, /*  構文エラー */

	// word token
	TK_WORD,        /* WORD 一般的な単語や文字列 */
	TK_SQUOTED_STR, // 展開されない
	TK_DQUOTED_STR, // 変数展開される

	// meta and op token
	TK_PIPE,         /* `|` */
	TK_HEREDOC,      /*`<<` */
	TK_REDIR_APPEND, /* `>>` */
	TK_REDIR_IN,     /* `<` */
	TK_REDIR_OUT,    /* `>` */
	TK_BLANK,        /* ` ` or `\t` */
	TK_NEWLINE,      /* `\n` */
	TK_EOF           /* \0 */

}						t_token_type;

typedef struct s_token
{
	enum e_token_type	type;
	char				*word;
	struct s_token		*next;
}						t_token;

// TODO: later to update
typedef enum e_word_flags
{
	WORD_SINGLE_QUOTE = 1 << 0,
	WORD_DOUBLE_QUOTE = 1 << 1,
	WORD_ESCAPE = 1 << 2,
	WORD_EXPANSION = 1 << 3,
	WORD_RESERVED = 1 << 4,
	WORD_ASSIGNMENT = 1 << 5,
	WORD_SYNTAX_ERROR = 1 << 6,
}						t_word_flags;

/* token -> word_list -> argv となる*/
typedef struct s_word_desc
{
	char				*word;
	enum e_word_flags	flags;
}						t_word_desc;

typedef struct s_word_list
{
	struct s_word_desc	*word;
	struct s_word_list	*next;
}						t_word_list;

typedef enum e_redir_type
{
	REDIR_EOF,
	REDIR_ERROR, // これは使わないかも
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

// expandがword_listを解決して、単一のwordに変換する
typedef struct s_redir
{
	enum e_redir_type	type;
	// filepath or here_doc word
	struct s_word_list	*word_list;
	// O_WRONLY | O_CREATE, O_RDONLY とかを入れる 元々mode
	int					oflag;
	// STDOUT_FILENO, STDIN_FILENO とかを入れる
	int					fd;
	bool				is_here_doc;
	bool				is_quoted_derimiter;
	struct s_redir		*next;
}						t_redir;

typedef enum e_cmd_type
{
	NONE,
	EXEC,
	PIPE
}						t_cmd_type;

typedef struct s_cmd
{
	enum e_cmd_type		type;
}						t_cmd;

typedef struct s_execcmd
{
	enum e_cmd_type		type;
	struct s_word_list	*word_list;
	struct s_redir		*redir_list;
}						t_execcmd;

typedef struct s_pipecmd
{
	enum e_cmd_type		type;
	struct s_cmd		*left;
	struct s_cmd		*right;
}						t_pipecmd;

#endif // STRUCT_H
