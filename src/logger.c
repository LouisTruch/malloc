#include "../inc/malloc.h"

void logger(int debug)
{
    // #ifdef LOG
    return;
    static int i = 0;
    switch (debug)
    {
    case HEAP_ALLOC:
        ft_printf("New heap allocation\n");
        break;
    case HEAP_DEALLOC:
        ft_printf("Heap deletion\n");
        break;
    case BLOCK_CREATION:
        ft_printf("New block created %i\n", i);
        break;
    case BLOCK_ATTRIBUTION:
        ft_printf("Block attributed\n");
        break;
    case BLOCK_RELEASE:
        ft_printf("Block released\n");
        break;
    case BLOCK_DIVISION:
        ft_printf("Dividing block\n");
        break;
    case MEM_DEFRAG:
        ft_printf("Defragmenting memory\n");
        break;
    }
    i++;
    // #endif
}