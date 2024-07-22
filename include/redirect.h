#ifndef REDIRECT_H
# define REDIRECT_H

# include "ast.h"
# include "minishell.h"
# include "token.h"

void	exec_redir(t_cmd *cmd, t_mgr *mgr);
int	ft_heredoc(const char *eof);
void ft_heredoc_output(int fd);


typedef struct s_fd_mgr
{
	int				fd;
	struct s_fd_mgr	*next;
}					t_fd_mgr;

#endif
