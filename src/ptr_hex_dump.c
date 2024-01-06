#include "../inc/malloc.h"

#include <stdio.h>
void block_hex_dump(t_chunk **ptr)
{
    t_chunk *block = *ptr;
    unsigned char *data = (unsigned char *)(block + sizeof(t_chunk));
    ft_dprintf(1, "Hex dump:\n");
    ft_dprintf(1, "data:%p\n", data);
}

void ptr_hex_dump(void *ptr)
{
    pthread_mutex_lock(&g_mutex);
    if (!ptr)
    {
        ft_putstr_fd("Error: block_hex_dump: NULL ptr", 2);
        goto end;
    }
    ptr -= sizeof(t_chunk);

    for (t_heap *current_heap = g_heap; current_heap; current_heap = current_heap->next)
    {
        // Can opti this cause of heap address' range
        // so we don't need to iter on every block of every heap
        for (t_chunk *current_block = current_heap->chunk; current_block; current_block = current_block->next)
        {
            if (current_block == ptr && !current_block->freed)
            {
                // here
                block_hex_dump(&current_block);
                goto end;
            }
        }
    }
    ft_putstr_fd("Error: block_hex_dump: ptr is not allocated", 2);

end:
    pthread_mutex_unlock(&g_mutex);
}