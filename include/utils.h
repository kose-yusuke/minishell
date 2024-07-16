/* utils.h - libft関数。 */
#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

void	ft_bzero(void *s, size_t n);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_strchr(const char *s, int c);


#endif