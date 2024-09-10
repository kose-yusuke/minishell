/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xlibc.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:44:57 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:44:58 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XLIBC_H
# define XLIBC_H

# include <stdlib.h>

void	*xmalloc(size_t bytes);
int		xclose(int fd);
int		xdup(int oldfd);
int		xdup2(int oldfd, int newfd);
pid_t	xfork(void);

#endif
