#include "../inc/malloc.h"

static int putchar_cnt(char c, int count)
{
    write(1, &c, 1);
    count++;
    return (count);
}

static int putstr_cnt(char *s, int count)
{
    if (!s)
    {
        count = putstr_cnt("(null)", count);
        return (count);
    }
    while (*s)
    {
        write(1, s, 1);
        s++;
        count++;
    }
    return (count);
}

static int basewrite_cnt(long long nb, char *base, int count)
{
    write(1, &base[nb], 1);
    count++;
    return (count);
}

static int putnbrbase_cnt(long long nb, char *base, int count)
{
    int base_l;

    base_l = ft_strlen(base);
    if (nb < 0)
    {
        count = putchar_cnt('-', count);
        nb *= -1;
    }
    if (nb < base_l)
        count = basewrite_cnt(nb, base, count);
    else
    {
        count = putnbrbase_cnt(nb / base_l, base, count);
        count = basewrite_cnt(nb % base_l, base, count);
    }
    return (count);
}

static int putunbrbase_cnt(unsigned long long nb, char *base, int count)
{
    unsigned long long base_l;

    base_l = ft_strlen(base);
    if (nb < base_l)
        count = basewrite_cnt(nb, base, count);
    else
    {
        count = putnbrbase_cnt(nb / base_l, base, count);
        count = basewrite_cnt(nb % base_l, base, count);
    }
    return (count);
}

static int manage_arg_p(unsigned long nb, int count)
{
    if (nb == 0)
        count = putstr_cnt("(nil)", count);
    else
    {
        count = putstr_cnt("0x", count);
        count = putunbrbase_cnt(nb, B_HEXL, count);
    }
    return (count);
}

static int to_convert(char f, va_list args, int count)
{
    if (f == 'c')
        count = putchar_cnt(va_arg(args, int), count);
    else if (f == 's')
        count = putstr_cnt(va_arg(args, char *), count);
    else if (f == 'p')
        count = manage_arg_p(va_arg(args, unsigned long long), count);
    else if (f == 'd' || f == 'i')
        count = putnbrbase_cnt(va_arg(args, int), B_DEC, count);
    else if (f == 'u')
        count = putunbrbase_cnt(va_arg(args, unsigned int), B_DEC, count);
    else if (f == 'x')
        count = putnbrbase_cnt(va_arg(args, unsigned int), B_HEXL, count);
    else if (f == 'X')
        count = putnbrbase_cnt(va_arg(args, unsigned int), B_HEXU, count);
    else if (f == '%')
        count = putchar_cnt('%', count);
    else
    {
        count = putchar_cnt('%', count);
        count = putchar_cnt(f, count);
    }
    return (count);
}

int ft_printf(const char *format, ...)
{
    va_list args;
    int count;

    va_start(args, format);
    count = 0;
    if (!format)
        return (-1);
    while (*format)
    {
        if (*format == '%')
        {
            count = to_convert(*(format + 1), args, count);
            format++;
        }
        else
            count = putchar_cnt(*format, count);
        format++;
    }
    va_end(args);
    return (count);
}
