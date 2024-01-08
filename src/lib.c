#include "../inc/malloc.h"

void *ft_memmove(void *dest, const void *src, size_t n)
{
    char *cstsrc;
    char *cstdest;
    size_t i;

    if (!dest && !src)
        return ((void *)0);
    i = 0;
    cstsrc = (char *)src;
    cstdest = (char *)dest;
    if (dest > src)
    {
        while (n-- > 0)
            cstdest[n] = cstsrc[n];
    }
    else
    {
        while (n > i)
        {
            cstdest[i] = cstsrc[i];
            i++;
        }
    }
    return ((void *)dest);
}

size_t ft_strlen(const char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
        i++;
    return (i);
}

void ft_putstr_fd(char *s, int fd)
{
    if (!s)
        return;
    write(fd, s, ft_strlen(s));
    return;
}

void ft_bzero(void *s, size_t n)
{
    char *casted;
    size_t i;

    casted = (char *)s;
    i = 0;
    while (n > i)
    {
        casted[i] = '\0';
        i++;
    }
    return;
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i;
    const unsigned char *str1;
    const unsigned char *str2;

    i = 0;
    str1 = (const unsigned char *)s1;
    str2 = (const unsigned char *)s2;
    while (str1[i] && str1[i] == str2[i])
        i++;
    return (str1[i] - str2[i]);
}

#include <limits.h>
static int ft_overflow(long rslt)
{
    if (rslt > INT_MAX)
        return (-1);
    else
        return (0);
}

int ft_atoi(const char *str)
{
    long rslt;
    int i;
    int sign;

    i = 0;
    sign = 1;
    rslt = 0;
    while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign *= -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        rslt = rslt * 10 + str[i] - '0';
        if (rslt * sign > INT_MAX || rslt * sign < INT_MIN)
            return (ft_overflow(rslt * sign));
        i++;
    }
    return (rslt * sign);
}
