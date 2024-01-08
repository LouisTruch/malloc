#include "../inc/malloc.h"

#include <ctype.h>

void chunk_hex_dump(t_chunk **ptr)
{
    t_chunk *chunk = *ptr;
    unsigned char *data = (unsigned char *)chunk + sizeof(t_chunk);
    char ascii[17];
    ft_bzero(ascii, sizeof(ascii));
    for (size_t i = 0; i < chunk->size - sizeof(t_chunk); i++)
    {
        // if (!i)
        //     ft_dprintf(1, "Chunk Metadata:\n");
        // if (i == sizeof(t_chunk))
        // {
        //     ft_dprintf(1, "User Data:\n");
        //     // break;
        // }
        if (i % 16 == 0)
            ft_dprintf(1, "%p: ", data + i);
        if (data[i] < 16)
            ft_dprintf(1, "0");
        ft_dprintf(1, "%x ", data[i]);
        if (isprint(data[i]))
            ascii[i % 16] = data[i];
        else
            ascii[i % 16] = '.';
        if (((i + 1) % 16) == 0)
            ft_dprintf(1, "%s\n", ascii);
    }
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
        for (t_chunk *current_chunk = current_heap->chunk; current_chunk; current_chunk = current_chunk->next)
        {
            if (current_chunk == ptr && !current_chunk->freed)
            {
                // here
                chunk_hex_dump(&current_chunk);
                goto end;
            }
        }
    }
    ft_putstr_fd("Error: block_hex_dump: ptr is not allocated", 2);

end:
    pthread_mutex_unlock(&g_mutex);
}