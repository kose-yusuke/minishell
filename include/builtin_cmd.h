/* builtins.h - シェルのビルトインコマンド (cd, echo, exit など) のプロトタイプと関連データの定義。 */
#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include <limits.h>

/* builtin.c */
bool	is_builtin(char *cmd_name);
int		exec_builtin(char **argv, t_mgr *mgr);
char	**convert_list_to_array(t_word *word_list);
void	print_argv(char **argv);

/* builtin_exit.c */
int		builtin_exit(char **argv);
/* builtin_export.c */
int		builtin_export(char **argv, t_mgr *mgr);
/* builtin_unset.c */
int		builtin_unset(char **argv, t_mgr *mgr);
/* builtin_pwd.c */
int		builtin_pwd(char **argv);
/* builtin_env.c */
int		builtin_env(char **argv, t_mgr *mgr, int var);
/* builtin_echo.c */
int		builtin_echo(char **argv);
/* builtin_cd.c */
int		builtin_cd(char **argv, t_mgr *mgr);

#endif
