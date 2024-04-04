/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 20:39:02 by sakitaha          #+#    #+#             */
/*   Updated: 2024/04/04 03:08:38 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TMP_H
# define TMP_H

# include "libft.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAXARGS 10

typedef enum e_cmd_type
{
	NONE,
	EXEC,
	REDIR,
	PIPE
}				t_cmd_type;

typedef struct s_cmd
{
	t_cmd_type	type;
}				t_cmd;

typedef struct s_execcmd
{
	t_cmd_type	type;
	char		*argv[MAXARGS];
	char		*eargv[MAXARGS];
}				t_execcmd;

typedef struct s_redircmd
{
	t_cmd_type	type;
	t_cmd		*cmd;
	char		*filepath;
	char		*efilepath;
	int oflag;     // O_WRONLY | O_CREATE, O_RDONLY とかを入れる 元々mode
	int fd;        // STDOUT_FILENO, STDIN_FILENO とかを入れる
	bool here_doc; // ここ? here_docはexpansionの時に展開する

}				t_redircmd;

typedef struct s_pipecmd
{
	t_cmd_type	type;
	t_cmd		*left;
	t_cmd		*right;
}				t_pipecmd;

void			error_exit(char *err_msg);
void			perror_exit(char *err_msg);

#endif
