/* utils.h - libft関数。 */
#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

/* utils.c */
void	ft_bzero(void *s, size_t n);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_strchr(const char *s, int c);
int		ft_isdigit(int c);

/* builtin.c ? */
size_t	ft_strlen(const char *str);
char	*ft_strndup(char *src, long len);
char	*ft_strdup(char *src);

// calloc
void	*ft_calloc(size_t count, size_t size);
// itoa
char	*ft_itoa(int n);

char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strcpy(char *dest, const char *src);

char	**ft_split(char const *s, char c);

#endif
