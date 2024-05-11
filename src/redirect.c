/*
#include <fcntl.h>
#include <unistd.h>
#include "minishell.h"

void redirect(int targetfd, char *filename)
{
	int filefd, stashed_targetfd;

	// 1. Redirect先のfdをopenする
	filefd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	filefd = stashfd(filefd); // filefdを退避させる

	// 2. Redirectする
	stashed_targetfd = stashfd(targetfd); // targetfdを退避させる
	if (filefd != targetfd)
	{
		dup2(filefd, targetfd); // filefdをtargetfdに複製する（元々のtargetfdは閉じられる）
		close(filefd);
	}

	// 3. コマンドを実行する
	exec_command();

	// 4. Redirectしていたのを元に戻す
	dup2(stashed_targetfd, targetfd); // 退避していたstashed_targetfdをtargetfdに複製する（元々のtargetfd）
}

int	stashfd(int fd)
{
	int	stashfd;

	stashfd = fcntl(fd, F_DUPFD, 10);
	if (stashfd < 0)
		fatal_error("fcntl");
	if (close(fd) < 0)
		fatal_error("close");
	return (stashfd);
}
stashfdという関数は引数として受け取ったfdを、他と被らないであろう10以上のfdに退避させています。 fcntl はminishellの使用可能関数には入っていませんが、簡単なので使用しています。後ほど他の使用可能関数を用いてstashfdを書き直すことはできるので、ひとまずはこんなものでいいでしょう。

- redirectが複数行われる際は、redirectしたのと逆の順番でreset_redirectをしていく必要があることに注意が必要です。例えば `echo hello >f1 >f2 >f3`  のようなredirectを行った時、どのようにfdが増えたり減ったり変化していき、最終的にどのようにresetされるかの流れは先ほどの章に書いた通り。


 */
