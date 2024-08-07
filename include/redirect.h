#ifndef REDIRECT_H
# define REDIRECT_H

# include "ast.h"
# include "minishell.h"
# include "token.h"

// void				ft_heredoc_output(int fd);
int					ft_heredoc(t_redir *redir, t_mgr *mgr);
void				exec_redir(t_redir *redir_list, t_mgr *mgr);

typedef struct s_fd_mgr
{
	int				fd;
	struct s_fd_mgr	*next;
}					t_fd_mgr;

#endif
