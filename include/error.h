/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:51:00 by sakitaha          #+#    #+#             */
/*   Updated: 2024/10/08 14:50:24 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "token.h"

void	memory_error_and_exit(char *func_name);
void	error_exit(const char *msg, int exit_status);
void	parser_error(t_token *unexpected_token);
void	report_error(char *cmd, char *file, char *info);
void	sys_error(char *cmd, char *msg);

#endif
