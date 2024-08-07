#ifndef REDIRECT_H
# define REDIRECT_H

# include "ast.h"
# include "minishell.h"
# include "token.h"

void				exec_redir(t_redir *redir_list, t_mgr *mgr);

/*
XXX: 使われていない。削除でいい？
typedef struct s_fd_mgr
{
	int				fd;
	struct s_fd_mgr	*next;
}					t_fd_mgr;
 */
#endif
