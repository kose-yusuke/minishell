/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakitaha <sakitaha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 02:44:37 by sakitaha          #+#    #+#             */
/*   Updated: 2024/09/11 02:44:40 by sakitaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRTOL_H
# define FT_STRTOL_H

# include <errno.h>
# include <limits.h>
# include <stdbool.h>
# include <stddef.h>

void	skip_space(const char **str);
bool	check_sign(const char **str);
bool	is_valid_digit(char c, int base);
long	ft_strtol(const char *str, char **endptr, int base);
#endif
