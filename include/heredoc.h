#ifndef HEREDOC_H
# define HEREDOC_H


# include "minishell.h"

# define FILE_TEMPLATE "/tmp/heredoc"
# define HEREDOC_MAX_FILES 10000

void	run_heredoc(t_cmd *cmd, t_mgr *mgr);
void	delete_tmp_files(void);
// void				ft_heredoc_output(int fd); <- heredoc関連なのでここに移動

#endif
