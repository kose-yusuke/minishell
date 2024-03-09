#　Minishell BNF　素案

## BNFとは何か
BNFは、文脈自由文法を定義するために用いられる形式言語

### BNFの基本構造
- 非終端記号::= 文法の構造を構成する抽象的な要素。通常、`<`と`>`で囲まれた名前
- 終端記号::= 文法中で変化しない、具体的な記号やキーワード
- 導出規則::= 非終端記号がどのように終端記号や他の非終端記号に展開されるかを定義する
（例::= `<sentence> ::=::== <subject> <predicate>`）
- 選択規則::= `|` を用いて、複数の導出規則の選択肢を示す

## BNFの案を考えていて浮かんだ未解決の疑問
- built-inと外部コマンドを識別して、対処をするのはExcecutionの役割ではないか
- BNFの定義において、built-inの名前を列挙しなくても良いのではないか

## MinishellのBNFの案（検討中）

### Minishellについて
minishellのmandatory は、pipe, redirect, heredoc, escape quote, built-in
つまり、|, <, >, <<, >>, ", ' と built-in の対応が必要
（bonusの課題はやらないので、&&, ||, , &, () は対応しなくて良い）

### BNFの例　（未完成）

```
<minishell>     ::= <pipeline> <linebreak>
                 | <linebreak>

<pipeline>      ::= <command>
                 | <pipeline> '|' <command>            /* 左結合のpipe */

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

<letter>        ::= [a-zA-Z0-9_./-]

<newline_list>  ::= '\n'
                 | <newline_list> '\n'

<linebreak>        ::= <newline_list>
                 | /* empty */
```
