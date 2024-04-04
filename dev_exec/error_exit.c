/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:28:41 by sakitaha          #+#    #+#             */
/*   Updated: 2024/03/26 20:44:37 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	error_exit(char *err_msg)
{
	// TODO: later to use ft_putstr_fd
	write(2, err_msg, strlen(err_msg));
	exit(EXIT_FAILURE);
}

void	perror_exit(char *err_msg)
{
	perror(err_msg);
	exit(EXIT_FAILURE);
}
