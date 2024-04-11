/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 03:24:01 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/30 13:52:27 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp.h"
#include <dirent.h>

/*
`minishell`プロジェクトでユーザーが入力したコマンド名からその実行可能ファイルのパスを解決するために使われる関数は、主に以下のものが考えられます：

- **`getenv`**: 環境変数`PATH`を取得するために使用します。`PATH`はシステムがコマンドを検索するディレクトリのリストを保持しています。

- **`access`**: 指定されたパスのファイルが存在し、実行可能であるかをチェックします。`access`関数は、ファイルが存在するかどうか、そして特定の権限（この場合は実行権限）があるかどうかを確認するために使用されます。

- **`opendir`, `readdir`,
	`closedir`**: これらの関数は、ディレクトリの内容を読み取り、特定のファイル（この場合はコマンド）を探すために使用されます。`PATH`環境変数にリストされたディレクトリを一つずつ`opendir`で開き、`readdir`でその中を検索し、処理が完了したら`closedir`で閉じます。

- **`stat`,
	`lstat`**: これらの関数は、ファイルのメタデータ（例えばファイルタイプやパーミッション）を取得します。ファイルが実際に実行可能なバイナリファイルであるかどうかを確認するために使用されます。

具体的な実装の流れは以下のようになります：

1. **`getenv`**を使用して`PATH`環境変数を取得します。
2. `PATH`に含まれる各ディレクトリに対して、以下の処理を行います：
   a. **`opendir`**でディレクトリを開きます。
   b. **`readdir`**を繰り返し使用して、ディレクトリ内の各ファイルをチェックします。
   c. ファイル名がユーザーが入力したコマンド名と一致した場合、**`stat`**または**`lstat`**を使用して、それが実行可能ファイルであるかどうかを確認します。
   d. 実行可能ファイルであれば、そのパスを組み立てます。パスの組み立てには、単純な文字列操作が使用されます（リストには含まれていませんが、`malloc`や`free`などでメモリ管理を行いつつ）。
   e. **`closedir`**でディレクトリを閉じます。
3. 実行可能ファイルのパスが見つかれば、そのパスを`execve`に渡して実行します。見つからなければ、適切なエラーメッセージを表示します（`perror`や`printf`などが使用される場合があります）。

このプロセスを通じて、`minishell`はユーザーが入力したコマンド名から実行可能ファイルを見つけ出し、それを実行するためのフルパスを特定することができます。

環境変数のPATHをsplitしたもの（PATHsとする）を保存しておいて、PATHsの各要素に対してstatでファイル名のリストをとってきて、入力されたコマンドと一致するものを探す

---

- **`access`**: 指定されたパスのファイルが存在し、実行可能であるかをチェックします。`access`関数は、ファイルが存在するかどうか、そして特定の権限（この場合は実行権限）があるかどうかを確認するために使用されます。

- **`opendir`, `readdir`,
	`closedir`**: これらの関数は、ディレクトリの内容を読み取り、特定のファイル（この場合はコマンド）を探すために使用されます。`PATH`環境変数にリストされたディレクトリを一つずつ`opendir`で開き、`readdir`でその中を検索し、処理が完了したら`closedir`で閉じます。

- **`stat`,
	`lstat`**: これらの関数は、ファイルのメタデータ（例えばファイルタイプやパーミッション）を取得します。ファイルが実際に実行可能なバイナリファイルであるかどうかを確認するために使用されます。

具体的な実装の流れは以下のようになります：

1. **`getenv`**を使用して`PATH`環境変数を取得します。
2. `PATH`に含まれる各ディレクトリに対して、以下の処理を行います：
   a. **`opendir`**でディレクトリを開きます。
   b. **`readdir`**を繰り返し使用して、ディレクトリ内の各ファイルをチェックします。
   c. ファイル名がユーザーが入力したコマンド名と一致した場合、**`stat`**または**`lstat`**を使用して、それが実行可能ファイルであるかどうかを確認します。
   d. 実行可能ファイルであれば、そのパスを組み立てます。パスの組み立てには、単純な文字列操作が使用されます（リストには含まれていませんが、`malloc`や`free`などでメモリ管理を行いつつ）。
   e. **`closedir`**でディレクトリを閉じます。
3. 実行可能ファイルのパスが見つかれば、そのパスを`execve`に渡して実行します。見つからなければ、適切なエラーメッセージを表示します（`perror`や`printf`などが使用される場合があります）。

このプロセスを通じて、`minishell`はユーザーが入力したコマンド名から実行可能ファイルを見つけ出し、それを実行するためのフルパスを特定することができます。
 */

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	destlen;
	size_t	srclen;

	destlen = 0;
	if (dest)
		destlen = ft_strlen(dest);
	if (size <= destlen)
		return (size + ft_strlen(src));
	while (*dest)
		dest++;
	srclen = ft_strlcpy(dest, src, size - destlen);
	return (destlen + srclen);
}

char	**get_path_list(void)
{
	char	*path;
	char	**path_list;

	// TODO: 失敗時にはexitでいいのかな？
	path = getenv("PATH");
	if (!path)
	{
		error_exit("Error: PATH not found\n");
	}
	path_list = ft_split(path, ':');
	if (!path_list)
	{
		error_exit("Error: ft_split failed (PATH)\n");
	}
	return (path_list);
}

char	*find_cmd(char *cmd_name)
{
	char			**path_list;
	char			*full_path;
	DIR				*dirp;
	struct dirent	*dp;
	struct stat		buf;
	int				i;
	int				result;

	result = access(cmd_name, X_OK); // cmd_nameがすでにフルパスで指定されている場合
	if (result == 0)
	{
		return (cmd_name);
	}
	i = 0;
	path_list = get_path_list();
	while (path_list[i])
	{
		dirp = opendir(path_list[i]);
		if (!dirp)
		{
			return (NULL);
		}
		while ((dp = readdir(dir)))
		{
			if (ft_strcmp(dp->d_name, cmd_name) == 0)
			{
				full_path = ft_strjoin(path_list[i], "/");
				full_path = ft_strjoin(full_path, cmd_name);
				if (stat(full_path, &buf) == 0)
				{
					if (buf.st_mode & S_IXUSR)
					{
						closedir(dir);
						return (full_path);
					}
				}
			}
		}
		closedir(dir);
		i++;
	}
	return (NULL);
}
