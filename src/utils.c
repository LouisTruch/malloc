#include "../inc/malloc.h"

int get_arena_size(size_t size)
{
    if (size <= TINY_ALLOC)
        return TINY;
    else if (size > TINY_ALLOC && size <= SMALL_ALLOC)
        return SMALL;
    else
        return LARGE;
}

void debug_a(void *ptr)
{
    return;
    static int pass = 0;
    ft_printf("DEBUG pass %i: %X\n", pass, ptr);
    pass++;
}