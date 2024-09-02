## statusコードまとめ(bashで挙動確認)
- 正常作動 : 0
- 異常 : 1

### 対応済み

### Exit status 未対応
- 引数なしのcatでCtrl + C : 130
```
bash-3.2$ cat
a
^C
bash-3.2$ echo $?
130
```




- 引数なしのcatでCtrl + \ : 131
```
bash-3.2$ cat
aaa
^\Quit: 3 ←Ctrl + \
bash-3.2$ echo $?
131
```
- パイプ時は一番右のコマンドで判断
```
bash-3.2$ echo aaa | cat a
cat: a: No such file or directory
bash-3.2$ echo $?
1
```
```
bash-3.2$ cat a | echo aaa
aaa
cat: a: No such file or directory
bash-3.2$ echo $?
0
```
- heredoc
```
Ctrl + Cで終了→1
Ctrl + DもしくはEOF wordで終了→0
Ctrl + \でやめる→そもそも作動しない
```
- syntax error : 258
```
bash-3.2$ >
bash: syntax error near unexpected token `newline'
bash-3.2$ echo $?
258
```
- command not found : 127
```
bash-3.2$ a
bash: a: command not found
bash-3.2$ echo $?
127
```
- 対応しなくても良さそうなやつ
```
bash-3.2$ .
bash: .: filename argument required
.: usage: . filename [arguments]
bash-3.2$ echo $?
2
```
```
bash-3.2$ /
bash: /: is a directory
bash-3.2$ echo $?
126
```

=========================================
previous exit status : 1
minishell$
=========================================
previous exit st
