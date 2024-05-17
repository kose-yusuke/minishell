/* builtins.h - シェルのビルトインコマンド (cd, echo, exit など) のプロトタイプと関連データの定義。 */
#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
/* builtin.c */
bool	is_builtin(t_execcmd *ecmd);
int		exec_builtin(t_execcmd *ecmd);

/* builtin_exit.c */
int builtin_exit(char **argv);

#endif
