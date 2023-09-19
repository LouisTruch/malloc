#include "../inc/malloc.h"

size_t align_mem(size_t size)
{
    return (size + (MEM_ALLIGN - 1)) & ~(MEM_ALLIGN - 1);
}

int get_arena_size(size_t size)
{
    if (size <= TINY_ALLOC)
        return TINY;
    else if (size > TINY_ALLOC && size <= SMALL_ALLOC)
        return SMALL;
    else
        return LARGE;
}