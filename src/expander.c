/* expand.c - 展開処理 */
#include "expander.h"

/**
 * expansion
 * 1. 環境変数の展開
 * 2. チルダの展開
 * 3. シングルクォート/ダブルクォートの展開
 * 4. エスケープの展開を行う
 *
 * チルダ（Tilde）Expansion
 * チルダ展開は、ユーザーホームディレクトリのパスに展開されます。例えば、`cd ~`や`cd ~/directory`のように使います。
 *
 * Variable and Parameter Exsion
 * 変数やパラメータの展開は、変数の値やシェルパラメータを展開するために使われます。
 * 例えば、`echo $HOME`はホームディレクトリのパスを出力します。
 *
 * Word Splitting
 * ワード分割は、展開された結果がスペース、タブ、改行によって分割され、複数のワードとして扱われるプロセスです。
 * これにより、例えば変数の値が複数の単語に分割されてコマンドに渡されます。
 *
 * Minishellプロジェクトの必須パートで、特に展開（expansion）に関わる部分を以下に挙げます：
 *

	* 1. **環境変数の展開**: ユーザーがコマンドラインに入力した環境変数（`$VAR`形式）を、その環境変数が持つ値に展開する必要があります。これはシェルスクリプトやコマンドライン操作において非常に一般的な操作です。

	* 2. **`$?`の展開**: これは特殊なケースの変数展開で、直前に実行されたコマンドの終了ステータスに展開されます。この機能は、スクリプトが前のコマンドの成功または失敗に基づいて条件分岐を行いたい場合に特に便利です。

	* 3. **クォート処理**: シングルクォート`'`とダブルクォート`"`は、文字列内の特定の文字や変数展開を制御するために使用されます。ダブルクォート内での`$`（環境変数の展開）は有効ですが、シングルクォート内では展開されません。この処理も展開の一種と考えることができます。

	* 4. **リダイレクションのヒアドキュメント（`<<`）**: ヒアドキュメントは、特定の終了デリミタまで標準入力からの入力を受け取り、それをコマンドの入力として使用します。この機能の実装には、入力されたテキストの処理や終了デリミタの検出など、特定の種類のテキスト処理（展開の一種）が必要になることがあります。
 *

	* これらの機能を実装する際には、コマンドラインから入力されたテキストの適切な解析と処理が必要になります。特に、環境変数の展開やクォートの処理は、シェルがコマンドラインをどのように解釈し、実行するプログラムにどのような引数を渡すかを決定する上で重要な役割を果たします。

	* これらの展開処理を正確に実装することで、ユーザーが期待するようなシェルの振る舞いを実現し、コマンドラインツールとしての使い勝手を大きく向上させることができます。

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
	int oflag; // O_WRONLY | O_CREATE, O_RDONLY とかを入れる 元々mode
	int fd;    // STDOUT_FILENO, STDIN_FILENO とかを入れる
}				t_redircmd;

typedef struct s_pipecmd
{
	t_cmd_type	type;
	t_cmd		*left;
	t_cmd		*right;
}				t_pipecmd;

void	expand_exec(t_execcmd *ecmd)
{
	// expansion
	// argvの中身をexpansionする
}

void	expand_redir(t_redircmd *rcmd)
{
	// expansion
	// redirの中身もexpansionする
	// heredocの展開もここで行う
	run_expansion(rcmd->cmd);
}

void	expand_pipe(t_pipecmd *pcmd)
{
	run_expansion(pcmd->left);
	run_expansion(pcmd->right);
}

void	run_expansion(t_cmd *cmd)
{
	if (cmd == 0)
	{
		exit(EXIT_SUCCESS); // return ;　もしくはexit(EXIT_FAILURE); なのかも
	}
	if (cmd->type == EXEC)
	{
		expand_exec((t_execcmd *)cmd);
	}
	else if (cmd->type == REDIR)
	{
		expand_redir((t_redircmd *)cmd);
	}
	else if (cmd->type == PIPE)
	{
		expand_pipe((t_pipecmd *)cmd);
	}
	exit(EXIT_SUCCESS);
}


// XXX: 参考->Usamiさんのコード
void append_char(char **s, char c)
{
    size_t size;
    char *new;
    size = 2;
    if (*s)
        size += strlen(*s);
    new = malloc(size);
    if (!new)
        fatal_error("malloc");
    if (*s)
        strlcpy(new, *s, size);
    new[size - 2] = c;
    new[size - 1] = '\0';
    if (*s)
        free(*s);
    *s = new;
}

// XXX: 参考->Usamiさんのコード
void	quote_removal(t_token *tok)
{
	char	*new_word;
	char	*p;
	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
    p = tok->word;
	new_word = NULL;
    while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
		{
			// skip quote
			p++;
			while (*p != SINGLE_QUOTE_CHAR)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				append_char(&new_word, *p++);
			}
			// skip quote
			p++;
		}
		else if (*p == DOUBLE_QUOTE_CHAR)
		{
			// skip quote
			p++;
			while (*p != DOUBLE_QUOTE_CHAR)
			{
				if (*p == '\0')
					assert_error("Unclosed single quote");
				append_char(&new_word, *p++);
			}
			// skip quote
			p++;
		}
		else
			append_char(&new_word, *p++);
	}
    free(tok->word);
	tok->word = new_word;
	quote_removal(tok->next);
}



void	expand(t_token *tok)
{
	quote_removal(tok);
}

// 他のcファイルから移動してきた。

// TODO: Usamiさんのコード
char *search_path(const char *filename)
{
    char path[PATH_MAX];
    char *value;
    char *end;

    value = getenv("PATH");
    while (*value)
    {
        bzero(path, PATH_MAX);
        end = strchr(value, ':');
        if (end)
			strncpy(path, value, end - value);
		else
			strlcpy(path, value, PATH_MAX);
		strlcat(path, "/", PATH_MAX);
		strlcat(path, filename, PATH_MAX);
        if (access(path, X_OK) == 0)
        {
            char *dup;
            dup = strdup(path);
            if (dup == NULL)
				fatal_error("strdup");
			return (dup);
        }
        if (end == NULL)
			return (NULL);
		value = end + 1;
    }
    return (NULL);
}
 */

