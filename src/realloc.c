#include "../inc/malloc.h"

static void *is_valid_ptr(void *ptr, t_heap **heap)
{
    t_block *block;
    while ((*heap))
    {
        block = (*heap)->block;
        while (block)
        {
            if (ptr == block)
                return ptr;
            block = block->next;
        }
        (*heap) = (*heap)->next;
    }
    return NULL;
}

void *realloc(void *ptr, size_t size)
{
    if (!ptr || !size)
        return (malloc(size));

    t_heap *heap = g_heap;
    t_block *block;
    ptr -= sizeof(t_block);
    if (!(block = is_valid_ptr(ptr, &heap)))
    {
        ft_putstr_fd("Realloc: invalid pointer\n", 2);
        return NULL;
    }
    if (block->size == align_mem(size + sizeof(t_block)))
        return (void *)block + sizeof(t_block);

    void *res_ptr = malloc(size);
    if (block->size > size)
        ft_memmove(res_ptr, ptr, size);
    else
        ft_memmove(res_ptr, ptr, block->size);
    free(ptr + sizeof(t_block));
    return res_ptr + sizeof(t_block);
}