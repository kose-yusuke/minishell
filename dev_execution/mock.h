/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:31:02 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/08 16:15:32 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOCK_H
# define MOCK_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <unistd.h>

// Parsed command representation
# define EXEC 1
# define REDIR 2
# define PIPE 3
# define HEREDOC 4

# define MAXARGS 10

// 手動のポリモーフィズムを実現するための構造体（基底型）
struct			cmd
{
	int			type;
};

// 各コマンドの構造体（特化型）
struct			execcmd
{
	int			type;
	char		*argv[MAXARGS];
	char		*eargv[MAXARGS];
};

struct			redircmd
{
	int			type;
	struct cmd	*cmd;
	char		*file;
	char		*efile;
	int			mode;
	int			fd;
};

struct			pipecmd
{
	int			type;
	struct cmd	*left;
	struct cmd	*right;
};

struct			listcmd
{
	int			type;
	struct cmd	*left;
	struct cmd	*right;
};

struct			backcmd
{
	int			type;
	struct cmd	*cmd;
};

#endif
