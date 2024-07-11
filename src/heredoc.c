/*
はい、正確にその通りです。ヒアドキュメント（heredoc）は、シェルスクリプト内でしばしば使われるリダイレクションの形式の一つで、入力を直接スクリプト内で指定することを可能にします。ヒアドキュメントの処理は、通常は親プロセスで行われます。これは、ヒアドキュメントの内容がコマンドの実行に先立って処理され、必要なデータがコマンドに供給されるためです。

### ヒアドキュメントの処理方法

1. **ヒアドキュメントのセットアップ**:
   - ヒアドキュメントが指定された場合、シェル（親プロセス）は、ヒアドキュメントで提供されたデータを一時的な入力ソースとして使用します。これには通常、一時ファイルの作成やメモリ上のバッファの使用が含まれることがあります。

2. **ファイルディスクリプタのリダイレクト**:
   - シェルは一時的な入力ソース（ヒアドキュメントの内容が保存されている場所）を標準入力（ファイルディスクリプタ0）にリダイレクトする必要があります。このリダイレクトは、ヒアドキュメントを読み取るためのコマンドが実行される前に設定されます。

3. **コマンドの実行**:
   - コマンドが実行されると、それはリダイレクトされた標準入力（ヒアドキュメントの内容）からデータを読み込みます。

4. **ファイルディスクリプタの復元**:
   - コマンドの実行後、標準入力を元の状態（通常はキーボードや別の入力ソース）に戻す必要があります。これは、後続のコマンドが正常に標準入力からデータを読み取れるようにするためです。

### 親プロセスでのリダイレクトの管理

親プロセスでヒアドキュメントを処理する際にファイルディスクリプタを適切に管理することは、プログラムが予期せぬ入力ソースからデータを読み込むのを防ぐために重要です。これにより、シェルスクリプトの予測可能性と安定性が向上します。

親プロセスでヒアドキュメントを扱う際の適切なリダイレクトとファイルディスクリプタの復元処理は、プログラムの全体的な動作において重要な役割を果たします。

bash-3.2$ ./a.out $foo
argv[0] = [./a.out]
argv[1] = [a]
argv[2] = [b]
bash-3.2$ ./a.out "$foo"
argv[0] = [./a.out]
argv[1] = [a    b]
bash-3.2$
以下は両方usamiさんのコードです。

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

int	read_heredoc(const char *delimiter)
{
	char	*line;
	int		pfd[2];

	if (pipe(pfd) < 0)
		fatal_error("pipe");
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		dprintf(pfd[1], "%s\n", line);
		free(line);
	}
	close(pfd[1]);
	return (pfd[0]);
}

char	*expand_heredoc_line(char *line)
{
	char	*new_word;
	char	*p;

	p = line;
	new_word = calloc(1, sizeof(char));
	if (new_word == NULL)
		fatal_error("calloc");
	while (*p)
	{
		if (is_variable(p))
			expand_variable_str(&new_word, &p, p);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p);
		else
			append_char(&new_word, *p++);
	}
	free(line);
	return (new_word);
}
 */
