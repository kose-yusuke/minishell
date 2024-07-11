/* builtins.h - シェルのビルトインコマンド (cd, echo, exit など) のプロトタイプと関連データの定義。 */
#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
#include <string.h>
/* builtin.c */
bool	is_builtin(t_execcmd *ecmd);
int		exec_builtin(t_execcmd *ecmd);

/* builtin_exit.c */
int builtin_exit(char **argv);
/* builtin_export.c */
int	builtin_export(char **argv);
/* builtin_unset.c */
int	builtin_unset(char **argv);
/* builtin_pwd.c */
int	builtin_pwd(char **argv);
/* builtin_env.c */
int	builtin_env(char **argv);
/* builtin_echo.c */
int	builtin_echo(char **argv);
/* builtin_cd.c */
int	builtin_cd(char **argv);

/* utils */
size_t	ft_strlen(const char *str);

#endif
