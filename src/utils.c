#include "../inc/malloc.h"

size_t align_mem(size_t size)
{
    return (size + (MEM_ALLIGN - 1)) & ~(MEM_ALLIGN - 1);
}
