#ifndef REDIRECT_H
# define REDIRECT_H

# include "ast.h"
# include "minishell.h"
# include "token.h"

void	exec_redir(t_execcmd *ecmd, t_mgr *mgr);



typedef struct s_fd_mgr
{
	int fd;
	struct s_fd_mgr	*next;
}						t_fd_mgr;

#endif
