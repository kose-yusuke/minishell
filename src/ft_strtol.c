#include "ft_strtol.h"

static int	confirm_base(const char **str, char **endptr, int base)
{
	if (base != 0 && base != 8 && base != 16)
		return (base);
	if (**str != '0')
	{
		if (base == 0)
			return (10);
		return (base);
	}
	(*str)++;
	if (endptr)
		*endptr = (char *)(*str);
	if ((**str == 'X' || **str == 'x') && (base == 0 || base == 16))
	{
		(*str)++;
		return (16);
	}
	if (base == 0 || base == 8)
		return (8);
	return (base);
}

static void	init_lookup_table(int char_to_value[], int base)
{
	int	c;

	c = 0;
	while (c < 256)
	{
		if ('0' <= c && c <= '9' && c < '0' + base)
		{
			char_to_value[c] = c - '0';
		}
		else if ('A' <= c && c <= 'Z' && c < 'A' + base - 10)
		{
			char_to_value[c] = 10 + c - 'A';
		}
		else if ('a' <= c && c <= 'z' && c < 'a' + base - 10)
		{
			char_to_value[c] = 10 + c - 'a';
		}
		else
		{
			char_to_value[c] = -1;
		}
		c++;
	}
}

static long	convert(const char *str, bool is_negative, char **endptr, int base)
{
	unsigned long	parsed_num;
	int				char_to_digit[256];

	parsed_num = 0;
	init_lookup_table(char_to_digit, base);
	if (char_to_digit[(unsigned char)*str] == -1)
		return (0);
	while (*str && char_to_digit[(unsigned char)*str] != -1)
	{
		parsed_num = parsed_num * base + char_to_digit[(unsigned char)*str++];
		if (parsed_num > (unsigned long)LONG_MAX + is_negative)
			errno = ERANGE;
	}
	if (endptr)
		*endptr = (char *)str;
	if (errno == ERANGE)
	{
		if (!is_negative)
			return (LONG_MAX);
		return (LONG_MIN);
	}
	if (is_negative)
		return ((long)(-1 * parsed_num));
	return ((long)parsed_num);
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	bool	is_negative;

	if (endptr)
		*endptr = (char *)str;
	if (!str || !*str)
	{
		errno = EINVAL;
		return (0);
	}
	skip_space(&str);
	is_negative = check_sign(&str);
	if (!is_valid_digit(*str, base) || base < 0 || base == 1 || base > 36)
	{
		errno = EINVAL;
		return (0);
	}
	base = confirm_base(&str, endptr, base);
	return (convert(str, is_negative, endptr, base));
}

/*
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void	test_strtol(const char *str, int base)
{
	char	*end;
	char	*ft_end;
	long	res;
	long	ft_res;

	printf("Testing: '%s' with base %d\n", str, base);
	errno = 0;
	res = strtol(str, &end, base);
	printf("Og_strtol : %ld, errno: %d, end: '%s'\n", res, errno, end);
	errno = 0;
	ft_res = ft_strtol(str, &ft_end, base);
	printf("ft_strtol : %ld, errno: %d, end: '%s'\n", ft_res, errno, ft_end);
	printf("-------------------------------------------------\n\n");
}

int	main(void)
{
	test_strtol("42", 0);
	test_strtol("0755", 0);
	test_strtol("0x1A", 0);
	test_strtol("101010", 2);
	test_strtol("-101010", 2);
	test_strtol("0436", 8);
	test_strtol("0xff", 16);
	test_strtol("123abc", 10);
	test_strtol("999999999999999999999999999999", 10);
	test_strtol("-999999999999999999999999999999", 10);
	test_strtol("0", 0);
	test_strtol("0x", 0);
	test_strtol("0xzz", 0);
	test_strtol("-z", 0);
	test_strtol("0z", 0);
	test_strtol("abc", 10);
	test_strtol("Z", 36);
	test_strtol("10", 36);
	test_strtol("", 10);
	test_strtol(" ", 10);
	test_strtol("-apple00", 10);
	test_strtol("123", 37);
	test_strtol("123", 1);
	return (0);
}

 */
