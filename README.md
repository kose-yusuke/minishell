# minishell

## Resources
- [The Architecture of Open Source Applications](https://aosabook.org/en/index.html)
- [AOSA 日本語版](https://m-takagi.github.io/aosa-ja/aosa.pdf)
- [GNU Bison](https://www.gnu.org/software/bison/manual/bison.html)
- [xv6](https://github.com/mit-pdos/xv6-public/tree/master)
- [The Open Group Base Specifications Issue 7, 2018 edition](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook)
- [minishell resources](https://minishell.simple.ink/)
- https://velog-io.translate.goog/@chez_bono/Minishell?_x_tr_sl=auto&_x_tr_tl=ja&_x_tr_hl=en-US&_x_tr_pto=wapp
- [man bash/和訳](https://ja.manpages.org/bash)
- [bash git](https://git.savannah.gnu.org/cgit/bash.git/)

## minishellの使用関数についてのメモ

readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tput

### 入力と履歴管理の関数

- `readline`: ユーザーからの入力を受け取り、動的にメモリを割り当てて文字列を返す
- `rl_clear_history`: `readline`によって保持されている全ての履歴項目を削除する
- `rl_on_new_line`: `readline`の内部状態を更新して、新しい行にカーソルが移動したことを伝える
- `rl_replace_line`: 現在の行の内容を指定したテキストで置換する
- `rl_redisplay`: 現在のコマンドラインバッファの内容を再描画する例えば、プログラムによる変更をユーザーに即座に表示したい場合に使用する
- `add_history`: 指定した文字列をコマンド履歴に追加するユーザーが入力したコマンドを後から参照できるようになる

### 出力とメモリ管理

- `printf`
- `malloc`
- `free`

### ファイルとディレクトリ操作

- `write`: ファイル記述子にデータを書き込む
- `access`: ファイルの存在やアクセス権限をチェックする
- `open`: ファイルを開いてファイル記述子を取得する
- `read`: ファイル記述子からデータを読み込む
- `close`: 開いたファイル記述子を閉じる
- `unlink`: ファイルのリンクを削除し、そのファイルを削除する（他にリンクがない場合）
- `opendir`: ディレクトリを開いて、ディレクトリストリームを取得する
- `readdir`: `opendir`で開いたディレクトリストリームからエントリを読み込む
- `closedir`: `opendir`で開いたディレクトリストリームを閉じる
- `getcwd`: 現在の作業ディレクトリのパスを取得する
- `chdir`: 作業ディレクトリを変更する

### プロセス管理

- `fork`: 新しいプロセスを作成する
- `wait`: 子プロセスが終了するのを待つ
- `waitpid`: 特定の子プロセスが終了するのを待つ
- `wait3`, `wait4`: `wait`の拡張版で、追加情報を提供する
- `execve`: プロセスに新しいプログラムをロードして実行する
- `exit`: プロセスを終了し、ステータスコードを返す
- `kill`: プロセスにシグナルを送信する

### シグナル管理
- `signal`
- `sigaction`
- `sigemptyset`
- `sigaddset`

### 入出力制御

- `dup`: 既存のファイル記述子を複製する
- `dup2`: 指定されたファイル記述子に既存のファイル記述子をコピーする
- `pipe`: ２つのファイル記述子を作成し、データのパイプラインを形成する
- `isatty`: ファイル記述子が端末に関連付けられているかどうかをチェックする
- `ttyname`: ファイル記述子に関連付けられた端末の名前を返す
- `ttyslot`: 端末デバイスのスロット番号を返す
- `ioctl`: デバイス固有の入出力操作を行う

### エラー処理
- `strerror`
- `perror`

### 環境変数
- `getenv`: 環境変数の値を取得する

### ターミナル制御
- `tcsetattr`: ターミナルの属性を設定する
- `tcgetattr`: ターミナルの現在の属性を取得する
- `tgetent`: 端末タイプのデータベースエントリを取得する
- `tgetflag`: ブール型の端末機能を取得する
- `tgetnum`: 数値型の端末機能を取得する
- `tgetstr`: 文字列型の端末機能を取得する
- `tgoto`: カーソル移動文字列を生成する
- `tputs`: 端末に制御文字列を出力する

### ファイル情報取得
- `stat`, `lstat`: ファイルの情報を取得する`lstat`はシンボリックリンク自体の情報を返す
- `fstat`: 開かれているファイル記述子に関連するファイルの情報を取得する


## BNFの例　（未完成）

<minishell>     ::= <pipeline> <linebreak>
                 | <linebreak>

<pipeline>      ::= <command>
                 | <pipeline> '|' <command>

<command>       ::= <cmd_prefix> <cmd_name> <cmd_suffix>
                 | <cmd_prefix> <cmd_name>
                 | <cmd_name> <cmd_suffix>
                 | <cmd_prefix>
                 | <cmd_name>

<cmd_name>      ::= <built-in> | <external_command>

<built-in>      ::= 'echo -n' | 'cd' | 'pwd' | 'export' | 'unset' | 'env' | 'exit'

<external_command> ::= <word>

<cmd_prefix>    ::=             <io_redirect>
                 | <cmd_prefix> <io_redirect>

<cmd_suffix>    ::=             <io_redirect>
                 | <cmd_suffix> <io_redirect>
                 | <word>
                 | <cmd_suffix> <word>

<redirect_list> ::=                <io_redirect>
                 | <redirect_list> <io_redirect>

<io_redirect>   ::=          <io_file>
                 | IO_NUMBER <io_file>
                 |           <io_here>
                 | IO_NUMBER <io_here>

<io_file>       ::= '<' <word>
                 |  '>' <word>
                 | '>>' <word>

<io_here>       ::= '<<' <word>

<quoted_string> ::= '"' <inside_quotes> '"'
                 | "'" <inside_quotes> "'"

<inside_quotes> ::= <word> | <white_space> <word>

<white_space>   ::= ' ' | '\t' | <white_space> <white_space>

<word>          ::= <letter> | <word> <letter>
                 | '$' <env_var>

<env_var>       ::= <letter> | <env_var> <letter>

<letter>        ::= [a-zA-Z0-9_./-]

<newline_list>  ::= '\n'
                 | <newline_list> '\n'

<linebreak>        ::= <newline_list>
                 | /* empty */


## 作業記録

### 展開（expansion）
#### 環境変数の展開
ユーザーがコマンドラインに入力した環境変数（`$VAR`形式）を、その値に展開する
例えば、`echo $HOME`はホームディレクトリのパスを出力する

#### `$?`の展開
`$?`は、直前に実行されたコマンドの終了ステータスに展開する
スクリプトが前のコマンドの成功や失敗に基づいて条件分岐を行う際に便利

#### クォートの処理
シングルクォート（`'`）とダブルクォート（`"`）は、文字列内の特定の文字や変数展開を制御する
ダブルクォート内の`$`（環境変数の展開）は有効だが、シングルクォート内では展開されない

#### ヒアドキュメント（Here Document）の処理
ヒアドキュメント（`<<`）は、特定の終了デリミタまで標準入力からの
入力を受け取り、それをコマンドの入力として使用する

#### ワード分割（Word Splitting）
ワード分割は、展開された結果がスペース、タブ、改行によって分割され、複数のワードとして扱われるプロセスこれにより、変数の値が複数の単語に分割されてコマンドに渡される

---

#### FDの管理について
- リソースの枯渇を防ぐため、ファイルディスクリプタ（FD）は適宜閉じる必要があるただし、すべてのFDをリストに記録して管理することは必ずしも必要ではない
- 子プロセスでは、`execve`を実行する直前にFDを開くことが推奨されるこの時、`O_CLOEXEC`フラグを指定することで、`execve`が成功した際に自動的にFDが閉じられる
- FDは展開（expansion）される際に`access`関数を用いてアクセス権を確認する
- `pipe`は親プロセスが閉じても子プロセスが生存している限り開いたままなので、親プロセスは`pipe`生成時に作成したFDを明示的に閉じる必要がある
- `pipe2()`が使用可能であれば`O_CLOEXEC`フラグを使用できるが、minishellで許可されている関数には含まれないため利用は難しい
- リダイレクトのみを行う子プロセスでは、プロセス終了直前に開いたすべてのFDを閉じる必要がある

---

#### hash関数

http://www.cse.yorku.ca/~oz/hash.html


### TODOリスト:

- [ ] 新しいコマンドを待つときにプロンプトを表示する。
- [ ] 正常に動作する履歴を持つ。
- [ ] 正しい実行ファイルを検索して起動する（PATH変数に基づくか、相対または絶対パスを使用）。
- [ ] シグナル受信を示すために複数のグローバル変数を使用しない。このアプローチは、シグナルハンドラーがメインのデータ構造にアクセスしないようにする。

- [x] クローズされていない引用符や必要とされていない特別な文字（バックスラッシュやセミコロンなど）を解釈しない。
- [ ] シングルクォート（'）を処理して、囲まれたテキストが解釈されないようにする。
- [ ] ダブルクォート（"）を処理して、囲まれたテキストのメタキャラクターが解釈されないようにする（$を除く）。

- [ ] リダイレクションを実装する：
    - [ ] `<` は入力をリダイレクトする。
    - [ ] `>` は出力をリダイレクトする。
    - [ ] `<<` は区切り文字が見つかるまで入力を読み込む（履歴は更新しない）。
    - [ ] `>>` は出力を追加モードでリダイレクトする。

- [ ] パイプ（|）を実装する。パイプライン内の各コマンドの出力は次のコマンドの入力に接続される。
- [ ] 環境変数（$の後に続く文字列）を扱い、その値に展開する。
- [ ] `$?`を扱い、最後に実行されたフォアグラウンドパイプラインの終了ステータスに展開する。
- [ ] ctrl-C、ctrl-D、ctrl-\ をBashのように扱う。

- [ ] インタラクティブモードでは：
    - [ ] ctrl-C は新しい行に新しいプロンプトを表示する。
    - [ ] ctrl-D はシェルを終了する。
    - [ ] ctrl-\ は何もしない。

- [ ] 以下のビルトインを実装する必要がある：
    - [ ] echo（-n オプション付き）
    - [ ] cd（相対または絶対パスのみ）
    - [ ] pwd（オプションなし）
    - [ ] export（オプションなし）
    - [ ] unset（オプションなし）
    - [ ] env（オプションや引数なし）
    - [ ] exit（オプションなし）


##### 作業メモ

- [ ] メモリ解放の設計の再検討
- [ ] heredocの実装
- [ ] execの実装
- [ ] waitのマクロの使い方
