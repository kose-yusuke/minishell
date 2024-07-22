#ifndef FT_STRTOL_H
# define FT_STRTOL_H

// # include "ft_ctype.h"
# include <errno.h>
# include <limits.h>
# include <stdbool.h>
# include <stddef.h>

void	skip_space(const char **str);
bool	check_sign(const char **str);
bool	is_valid_digit(char c, int base);
long	ft_strtol(const char *str, char **endptr, int base);
#endif
