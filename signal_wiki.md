## statusコードまとめ(bashで挙動確認)
- 正常作動 : 0
- 異常 : 1

bashの実装を参照して、いろいろなexit status codeをenumに
まとめました。多くは不要になるので、あとで消します
```c
typedef enum e_status
{
	SC_SUCCESS = 0,        // 正常終了
	SC_FAILURE = 1,        // 一般的なエラー
	SC_BADUSAGE = 2,       // ビルトインコマンドの誤用
	SC_MISCERROR = 2,      // その他のエラー
	SC_RETRYFAIL = 124,    // 再試行の失敗
	SC_WEXPCOMSUB = 125,   // コマンド置換に失敗
	SC_BINARY_FILE = 126,  // 実行できないバイナリファイル
	SC_NOEXEC = 126,       // 実行できないコマンド
	SC_NOINPUT = 126,      // 入力がない
	SC_NOTFOUND = 127,     // コマンドが見つからない
	SC_FATAL_ERROR = 255,  // 致命的なエラー
	SC_SHERRBASE = 256,    // すべての特別なエラー値はこれより大きい
	SC_BADSYNTAX = 257,    // シェル構文エラー
	SC_USAGE = 258,        // 使用方法の誤り
	SC_REDIRFAIL = 259,    // リダイレクトに失敗
	SC_BADASSIGN = 260,    // 変数の代入エラー
	SC_EXPFAIL = 261,      // ワード展開に失敗
	SC_DISKFALLBACK = 262, // ディスクコマンドへのフォールバック
}								t_status;
```
### 対応済

- 引数なしのcatでCtrl + C : 130 (128 + signal number)
```sh
bash-3.2$ cat
a
^C
bash-3.2$ echo $?
130
```
```sh
minishell$ cat
a
^C
minishell$ echo $?
130
```
---
- 引数なしのcatでCtrl + \ : 131
```sh
bash-3.2$ cat
aaa
^\Quit: 3 ←Ctrl + \
bash-3.2$ echo $?
131
```
```sh
minishell$ cat
aaa
aaa
^\Quit: 3
minishell$ echo $?
131
```
---
- パイプ時は一番右のコマンドで判断
```sh
bash-3.2$ echo aaa | cat a
cat: a: No such file or directory
bash-3.2$ echo $?
1
```
```sh
minishell$ echo aaa | cat a
cat: a: No such file or directory
minishell$ echo $?
1
```
---
```sh
bash-3.2$ cat a | echo aaa
aaa
cat: a: No such file or directory
bash-3.2$ echo $?
0
```
```sh
minishell$ cat a | echo aaa
aaa
cat: a: No such file or directory
minishell$ echo $?
0
```
---
- heredoc
```
Ctrl + Cで終了→1
Ctrl + DもしくはEOF wordで終了→0
Ctrl + \でやめる→そもそも作動しない
```
---
- syntax error : 258
```sh
bash-3.2$ >
bash: syntax error near unexpected token `newline'
bash-3.2$ echo $?
258
```
```sh
minishell$ >
minishell: syntax error near unexpected token `newline'
minishell$ echo $?
258
```
---
- command not found : 127
```sh
bash-3.2$ a
bash: a: command not found
bash-3.2$ echo $?
127
```
```sh
minishell$ a
minishell: a: command not found
minishell$ echo $?
127
```
---

### 未対応

- 要対応のもの
```sh
bash-3.2$ cd testdir/
bash-3.2$ echo $?
0
```
```sh
minishell$ cd testdir/
minishell$ echo $?
1
```

- 対応しなくても良さそうなやつ
```sh
bash-3.2$ .
bash: .: filename argument required
.: usage: . filename [arguments]
bash-3.2$ echo $?
2
```
```sh
minishell$ .
minishell: .: is a directory
minishell$ echo $?
1
```
---
```sh
bash-3.2$ /
bash: /: is a directory
bash-3.2$ echo $?
126
```
```sh
minishell$ /
minishell: /: is a directory
minishell$ echo $?
1
```

