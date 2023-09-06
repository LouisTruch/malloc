#include "../inc/malloc.h"

void my_free(void *ptr)
{
    ptr -= sizeof(t_block);
    ft_printf("Free: %X\n", ptr);

    t_heap *heap = g_heap;
    t_block *block = NULL;
    while (heap)
    {
        // Can opti this cause I know heap address' range
        block = heap->block;
        while (block)
        {
            if (block == ptr)
            {
                ft_printf("FOUND\n");
            }
            block = block->next;
        }
        heap = heap->next;
    }
}