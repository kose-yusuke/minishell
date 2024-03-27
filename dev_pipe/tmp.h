/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 20:39:02 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/27 21:24:52 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TMP_H
# define TMP_H

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
	// command and arguments
	char		*argv[MAXARGS];
}				t_execcmd;

typedef struct s_redircmd
{
	t_cmd_type	type;
	t_cmd		*cmd;
	char		*filepath;
	// O_WRONLY | O_CREATE, O_RDONLY とかを入れるための変数。正しいフラグを入れる必要がある。
	int			oflag;
	// STDOUT_FILENO, STDIN_FILENO とかを入れるための変数
	int			fd;
	// ここは必要なのかな? here_docはどうやって使うのか、現時点では不明。
	bool		here_doc;

}				t_redircmd;

typedef struct s_pipecmd
{
	t_cmd_type	type;
	t_cmd		*left;
	t_cmd		*right;
}				t_pipecmd;

#endif
