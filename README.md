# Minishell

## 目次
- [使用した資料（Resources）](#使用した資料resources)
- [使用可能な関数一覧と説明](#使用可能な関数一覧と説明)
- [TODOリスト](#todoリスト)

## 使用した資料（Resources）
- [minishellの作り方](https://usatie.notion.site/minishell-29921d3ea13447ad897349acd5733d5e#2ebba0b5890641898a16957b69e50f1f)
- [The Architecture of Open Source Applications](https://aosabook.org/en/index.html)
- [AOSA 日本語版](https://m-takagi.github.io/aosa-ja/aosa.pdf)
- [GNU Bison](https://www.gnu.org/software/bison/manual/bison.html)
- [xv6](https://github.com/mit-pdos/xv6-public/tree/master)
- [The Open Group Base Specifications Issue 7, 2018 edition](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook)
- [Minishell resources](https://minishell.simple.ink/)
- [minishellに関する記事](https://velog-io.translate.goog/@chez_bono/Minishell?_x_tr_sl=auto&_x_tr_tl=ja&_x_tr_hl=en-US&_x_tr_pto=wapp)
- [man bash/和訳](https://ja.manpages.org/bash)
- [bash git](https://git.savannah.gnu.org/cgit/bash.git/)
- [GNU Readline Library](https://tiswww.cwru.edu/php/chet/readline/readline.html)
- [Appendix E. Exit Codes With Special Meanings](https://tldp.org/LDP/abs/html/exitcodes.html#EXITCODESREF)
- [POSIX specification](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_06) <- 変数名の仕様について
## 使用可能な関数一覧と説明

### 入力と履歴管理の関数
- `readline`: ユーザーからの入力を受け取り、動的にメモリを割り当てて文字列を返す
- `rl_clear_history`: `readline`によって保持されている全ての履歴項目を削除する
- `rl_on_new_line`: `readline`の内部状態を更新して、新しい行にカーソルが移動したことを伝える
- `rl_replace_line`: 現在の行の内容を指定したテキストで置換する
- `rl_redisplay`: 現在のコマンドラインバッファの内容を再描画する
- `add_history`: 指定した文字列をコマンド履歴に追加する

### 出力とメモリ管理
- `printf`: 形式付き出力を行う
- `malloc`: メモリを動的に割り当てる
- `free`: メモリを解放する

### ファイルとディレクトリ操作
- `write`: ファイル記述子にデータを書き込む
- `access`: ファイルの存在やアクセス権限をチェックする
- `open`: ファイルを開いてファイル記述子を取得する
- `read`: ファイル記述子からデータを読み込む
- `close`: 開いたファイル記述子を閉じる
- `unlink`: ファイルのリンクを削除し、そのファイルを削除する
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
- `signal`: シグナルを設定する
- `sigaction`: シグナルの動作を変更する
- `sigemptyset`: シグナルセットを初期化する
- `sigaddset`: シグナルセットにシグナルを追加する

### 入出力制御
- `dup`: 既存のファイル記述子を複製する
- `dup2`: 指定されたファイル記述子に既存のファイル記述子をコピーする
- `pipe`: ２つのファイル記述子を作成し、データのパイプラインを形成する
- `isatty`: ファイル記述子が端末に関連付けられているかどうかをチェックする
- `ttyname`: ファイル記述子に関連付けられた端末の名前を返す
- `ttyslot`: 端末デバイスのスロット番号を返す
- `ioctl`: デバイス固有の入出力操作を行う

### エラー処理
- `strerror`: エラーメッセージを返す
- `perror`: 標準エラー出力にエラーメッセージを出力する

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
- `stat`: ファイルの情報を取得する
- `lstat`: シンボリックリンク自体の情報を返す
- `fstat`: 開かれているファイル記述子に関連するファイルの情報を取得する

## TODOリスト
- [x] 新しいコマンドを待つときにプロンプトを表示する
- [x] 正常に動作する履歴を持つ
- [x] 正しい実行ファイルを検索して起動する（PATH変数に基づくか、相対または絶対パスを使用）
- [x] シグナル受信を示すために複数のグローバル変数を使用しない。このアプローチは、シグナルハンドラーがメインのデータ構造にアクセスしないようにする
- [x] クローズされていない引用符や必要とされていない特別な文字（バックスラッシュやセミコロンなど）を解釈しない
- [x] シングルクォート（'）を処理して、囲まれたテキストが解釈されないようにする
- [x] ダブルクォート（"）を処理して、囲まれたテキストのメタキャラクターが解釈されないようにする（$を除く）
- [x] リダイレクションを実装する：
    - [x] `<` は入力をリダイレクトする
    - [x] `>` は出力をリダイレクトする
    - [x] `<<` は区切り文字が見つかるまで入力を読み込む（履歴は更新しない）
    - [x] `>>` は出力を追加モードでリダイレクトする
- [x] パイプ（|）を実装する。パイプライン内の各コマンドの出力は次のコマンドの入力に接続される
- [x] 環境変数（$の後に続く文字列）を扱い、その値に展開する
- [x] `$?`を扱い、最後に実行されたフォアグラウンドパイプラインの終了ステータスに展開する
- [x] ctrl-C、ctrl-D、ctrl-\ をBashのように扱う
- [x] インタラクティブモードでは：
    - [x] ctrl-C は新しい行に新しいプロンプトを表示する
    - [x] ctrl-D はシェルを終了する
    - [x] ctrl-\ は何もしない
- [x] 以下のビルトインを実装する必要がある：
    - [x] echo（-n オプション付き）
    - [x] cd（相対または絶対パスのみ）
    - [x] pwd（オプションなし）
    - [x] export（オプションなし）
    - [x] unset（オプションなし）
    - [x] env（オプションや引数なし）
    - [x] exit（オプションなし）
