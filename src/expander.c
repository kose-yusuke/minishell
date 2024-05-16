/* expand.c - 展開処理 */
#include "expander.h"

/*
にゃんこgptが君を見守ってるから、安心してexpansionを書いていこうにゃ！以下のアドバイスで進めてみてね：

### Expansionの基本的な流れ

1. **環境変数の展開**
   - コマンドラインで使用される環境変数（例: `$HOME`）を正しい値に置き換える。
   - `getenv` 関数を使って環境変数の値を取得し、文字列を置き換える。

2. **引用符の処理**
   - シングルクオート `' '` で囲まれた文字列は、そのまま扱う（メタ文字を解釈しない）。
   - ダブルクオート `" "` で囲まれた文字列は、 `$` などの一部のメタ文字を解釈する。

3. **エスケープシーケンス**
   - バックスラッシュ `\` を使ったエスケープシーケンスの解釈。

### コード例

以下は、簡単な環境変数展開の例だにゃ：

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *expand_variable(const char *str) {
    if (str[0] != '$') {
        return strdup(str);
    }
    const char *var_name = str + 1;
    const char *var_value = getenv(var_name);
    return var_value ? strdup(var_value) : strdup("");
}

int main() {
    const char *input = "$HOME";
    char *expanded = expand_variable(input);
    printf("Expanded: %s\n", expanded);
    free(expanded);
    return 0;
}
```

### ユーモアと励まし

にゃんこgptは君を全力で応援するにゃ！頑張ってる姿を見ると、にゃんこgptも元気が出るんだにゃ！君のシェルがバグなく動くことを祈ってるにゃ〜！失敗しても大丈夫、にゃんこgptがいつでもサポートするから、安心してチャレンジしてにゃ！一歩ずつ進んでいけば、きっと素晴らしいシェルが完成するにゃ！がんばれ〜だにゃ〜！
expander機能を実装する際には、以下のステップに従って進めると良いでしょう。それぞれの機能を段階的に実装して、テストしながら進めることで、確実に動作するようにすることができます。

### 実装のステップ

1. **環境変数の展開**:
   - まずは、`$VAR`形式の環境変数をその値に展開する機能を実装します。
   - 関数名の例：`expand_env_variables`

2. **チルダ（Tilde）の展開**:
   - 次に、`~`をユーザーのホームディレクトリに展開する機能を追加します。
   - 関数名の例：`expand_tilde`

3. **クォートの処理**:
   - シングルクォートとダブルクォートの処理を実装します。特に、ダブルクォート内での変数展開に注意します。
   - 関数名の例：`handle_quotes`

4. **ヒアドキュメント（Here Document）の処理**:
   - ヒアドキュメントの処理を実装し、特定のデリミタまでの入力を受け取る機能を追加します。
   - 関数名の例：`handle_heredoc`

5. **ワード分割（Word Splitting）**:
   - 最後に、展開された結果をスペース、タブ、改行によって分割する処理を追加します。
   - 関数名の例：`word_splitting`

### 各関数の名前と役割

1. **環境変数の展開**:
   ```c
   char *expand_env_variables(const char *input);
   ```

2. **チルダ（Tilde）の展開**:
   ```c
   char *expand_tilde(const char *input);
   ```

3. **クォートの処理**:
   ```c
   char *handle_quotes(const char *input);
   ```

4. **ヒアドキュメント（Here Document）の処理**:
   ```c
   char *handle_heredoc(const char *input, const char *delimiter);
   ```

5. **ワード分割（Word Splitting）**:
   ```c
   char **word_splitting(const char *input);
   ```

### 実装例

以下は、環境変数の展開を実装する際の例です：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*expand_env_variables(const char *input) {
	if (input[0] == '$') {
		return (getenv(input + 1));
	}
	return (NULL);
}

int	main(void) {
	const char *test_input = "$HOME";
	char *expanded = expand_env_variables(test_input);
	if (expanded) {
		printf("Expanded: %s\n", expanded);
	} else {
		printf("No expansion\n");
	}
	return (0);
}
```

この例では、入力が`$`で始まる場合に、その環境変数の値を取得して返します。

次のステップとしては、各機能を上記の順序で実装し、それぞれをテストしながら進めることです。また、全体の統合テストも行い、各機能が一緒に動作することを確認します。

もし具体的な実装のアドバイスやコードレビューが必要であれば、遠慮なく聞いてくださいにゃん。

以下は、具体的なコード例です：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*expand_variable(const char *input) {
	if (input[0] == '$') {
		return (getenv(input + 1));
	}
	return (NULL);
}

char	*expand_tilde(const char *input) {
	if (input[0] == '~') {
		char *home = getenv("HOME");
		char *expanded = malloc(strlen(home) + strlen(input));
		strcpy(expanded, home);
		strcat(expanded, input + 1);
		return (expanded);
	}
	return (NULL);
}

void	handle_quotes(const char *input) {
	// シングルクォートとダブルクォートの処理
}

void	handle_escapes(const char *input) {
	// バックスラッシュでエスケープされた文字の処理
}

void	handle_heredoc(const char *input) {
	// ヒアドキュメントの処理
}

int	main(void) {
	char input[] = "$HOME/test";
	char *expanded = expand_variable(input);
	if (expanded) {
		printf("Expanded: %s\n", expanded);
	}
	// 他の展開処理を呼び出す
	return (0);
}
 */



/*

#include "expansion.h"
#include <stdlib.h>
#include <string.h>

// 環境変数を展開する関数の例
char	*expand_variable(const char *input)
{
	if (input[0] == '$')
		return (getenv(input + 1));
	return (strdup(input));
}

// チルダを展開する関数の例
char	*expand_tilde(const char *input)
{
	char	*home;
	char	*expanded;

	if (input[0] == '~')
	{
		home = getenv("HOME");
		expanded = malloc(strlen(home) + strlen(input));
		strcpy(expanded, home);
		strcat(expanded, input + 1);
		return (expanded);
	}
	return (strdup(input));
}

// ワードリストを展開する関数
void	expand_word_list(t_word *word_list)
{
	t_token	*token;
	char	*expanded;

	while (word_list)
	{
		token = word_list->token;
		if (token->type == TK_WORD)
		{
			expanded = expand_variable(token->word);
			free(token->word);
			token->word = expanded;
		}
		else if (token->type == TK_SQUOTE || token->type == TK_DQUOTE)
		{
			// クォート内の展開処理
		}
		word_list = word_list->next;
	}
}



 */

// ダミー関数
void	expand_word_list(t_word *word_list)
{
	// ここに環境変数やチルダの展開の実装を追加
	while (word_list)
	{
		// word_list->token を展開
		word_list = word_list->next;
	}
}

void	expand_redir_list(t_redir *redir_list)
{
	// ここにリダイレクトのための展開の実装を追加
	while (redir_list)
	{
		expand_word_list(redir_list->word_list);
		redir_list = redir_list->next;
	}
}

void	expand_exec(t_execcmd *ecmd)
{
	expand_word_list(ecmd->word_list);
	expand_redir_list(ecmd->redir_list);
}

void	expand_pipe(t_pipecmd *pcmd)
{
	run_expansion(pcmd->left);
	run_expansion(pcmd->right);
}

void	run_expansion(t_cmd *cmd)
{
	if (cmd == NULL || cmd->type == NONE)
		return ;
	else if (cmd->type == EXEC)
		expand_exec((t_execcmd *)cmd);
	else if (cmd->type == PIPE)
		expand_pipe((t_pipecmd *)cmd);
	else
		exit(EXIT_FAILURE);
}

/**

void	expand_exec(t_execcmd *ecmd)
{
	// expansion
	// argvの中身をexpansionする
}




// XXX: 参考->Usamiさんのコード
void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

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
char	*search_path(const char *filename)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;
			char *dup;

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
