#include "../inc/malloc.h"

void logger(int debug)
{
    // #ifdef LOG
    switch (debug)
    {
    case HEAP_ALLOC:
        ft_printf("New heap allocation\n");
        break;
    case HEAP_DEALLOC:
        ft_printf("Heap deletion\n");
        break;
    case BLOCK_CREATION:
        ft_printf("New block created\n");
        break;
    case BLOCK_ATTRIBUTION:
        ft_printf("Block attributed\n");
        break;
    case BLOCK_RELEASE:
        ft_printf("Block released\n");
        break;
    }
    // #endif
}