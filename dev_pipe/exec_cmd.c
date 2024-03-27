/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 20:27:32 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/27 22:43:33 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp.h"

/*
execve関数の引数

	第1引数: 実行するプログラムへのパス。これは実行ファイルをファイルシステム上で特定するために使用
	第2引数（argv）: プログラムに渡される引数の配列。この配列の最初の要素（argv[0]）は、慣例として
	実行するプログラムの名前が設定されます。これはプログラムが自身の名前を知るために使われ、
	コマンドラインから実行された場合のコマンド名（またはスクリプト名など）を反映します。
	配列の残りの要素には、実際のコマンドライン引数（オプション、フラグ、その他の引数）が格納されます。

TODO: userが入力したコマンド名に対して、pathを解決する処理が必要

1. 直接のパス指定がある場合
	検索は不要

2. PATH環境変数の利用
	ユーザーがコマンド名のみを指定した場合（例：`ls`）
	シェルは`PATH`環境変数に設定されたディレクトリのリストを使用して、実行可能なプログラムのフルパスを検索

3. パスの解決
	頻繁に使用されるパスをキャッシュすることで、パフォーマンスを向上させられる
	定番のパスは、`/bin`、`/usr/bin`、`/usr/local/bin`など
4. エラーハンドリング
*/

void	exec_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	if (cmd->type != EXEC)
	{
		error_exit("Error: run_cmd failed\n");
	}
	ecmd = (t_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
	{
		exit(0);
	}
	// TODO: ここでpathの解決を行う
	// TODO: 環境変数はまだ実装していないから第三引数がNULLになっているよ
	if (execve(ecmd->argv[0], ecmd->argv, NULL) == -1)
	{
		perror_exit("Error: execve failed\n");
	}
	// execveが成功すると、この以下のコードは実行されません
	error_exit("Error: run_cmd failed\n");
}

void	run_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	if (cmd->type == EXEC)
	{
		exec_cmd(cmd);
	}
	else if (cmd->type == REDIR)
	{
		exec_redir(cmd);
	}
	else if (cmd->type == PIPE)
	{
		exec_pipe(cmd);
	}
	else
	{
		error_exit("Error: run_cmd failed\n");
	}
}
