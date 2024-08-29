#ifndef HEREDOC_H
# define HEREDOC_H

# include "error.h"
# include "minishell.h"
# include <errno.h>

# define FILE_TEMPLATE "/tmp/heredoc"
# define HEREDOC_MAX_FILES 10000

/* heredoc_file.c */
int		prepare_tmp_file(char **file_name);
void	delete_tmp_files(void);

/* heredoc.c */
void	ft_heredoc(t_redir *redir, t_mgr *mgr);

// void				ft_heredoc_output(int fd); <- heredoc関連なのでここに移動

#endif
