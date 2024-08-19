/* xlibc.h - 標準Cライブラリのラッパー関数 */
#ifndef XLIBC_H
# define XLIBC_H

# include <stdlib.h>

void	*xmalloc(size_t bytes);
int		xdup(int oldfd);
int		xdup2(int oldfd, int newfd);

#endif
