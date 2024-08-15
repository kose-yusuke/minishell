/* utils.h - libft関数。 */
#ifndef UTILS_H
# define UTILS_H

void	ft_bzero(void *s, size_t n);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *str);
char	*ft_strndup(char *src, long len);
char	*ft_strdup(char *src);

#endif
