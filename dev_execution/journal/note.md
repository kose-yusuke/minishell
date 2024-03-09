# dev_execution/note.md

## Introduction

今後の予定と参考資料のメモ

## 予定

- [] モックの作成
	- [] 入力を受け取る
	- [] 入力をトークンに分割する
	- [] トークンをパースしてASTを作成する
- [] executionロジックの簡易実装
- [] ユニットテストの作成
- [] ユニットテストの実行

## Resources

### shell全体
[xv6](https://github.com/mit-pdos/xv6-public/tree/master) : xv6のshellの実装を参考にする。
https://usatie.notion.site/minishell-29921d3ea13447ad897349acd5733d5e#8098374831bf4f4298b8f0545944f5db : usamiさんのnotion

### lexer
#### token と shell grammar
[The Open Group Base Specifications Issue 7, 2018 edition](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)

#### minishell lexerについての記事
https://liberius.notion.site/Tokenization-process-How-you-shouldn-t-do-lexer-for-minishell-d7d832fcba0d4955a159a40f9ece8110
https://qiita.com/ToYeah0102/items/4c980440ed790d110a7f


### parser

### expansion

### execution

### test cases
https://docs.google.com/spreadsheets/d/1TDwyd-S0WBAXehgkrKQtBJ6zquQ4p6k7JfE5g3jICNA/edit?pli=1#gid=0
https://docs.google.com/spreadsheets/u/0/d/1uJHQu0VPsjjBkR4hxOeCMEt3AOM1Hp_SmUzPFhAH-nA/htmlview?lsrp=1#gid=0

### 未読資料
[The Architecture of Open Source Applications](https://aosabook.org/en/index.html)
[GNU Bison](https://www.gnu.org/software/bison/manual/bison.html)
[xv6 資料](https://ocw.mit.edu/courses/6-828-operating-system-engineering-fall-2012/3def8fcd397933ebb846fb479bdcf556_MIT6_828F12_xv6-book-rev7.pdf)

### 資料リスト
https://minishell.simple.ink/

### テスター
[Minishell Tester](https://github.com/solaldunckel/minishell-tester)
