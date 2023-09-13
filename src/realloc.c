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

static void *try_adjacent_block(t_heap **heap, t_block **block, const size_t new_size)
{
    if ((*block)->next && (*block)->next->size >= new_size - (*block)->size)
    {
        (*block)->size = new_size + sizeof(t_block);
        (*block)->next = (*block)->next->next;
        return *block;
    }
    return NULL;
}

static void *try_filling_heap(t_heap **heap, t_block **block, const size_t new_size)
{
    if (!(*block)->next && (new_size - (*block)->size >= (*heap)->free_space))
    {
        (*heap)->free_space -= (new_size - (*block)->size);
        (*block)->size = new_size + sizeof(t_block);
        return *block;
    }
    return NULL;
}

static void divide_block(t_heap **heap, t_block **block)
{
    // Can probably call divide_block from malloc.c
}

void *my_realloc(void *ptr, size_t size)
{
    if (!ptr || !size)
        return NULL;

    size = align_mem(size);
    ptr -= sizeof(t_block);
    t_heap *heap = g_heap;
    t_block *block;
    if (!(block = is_valid_ptr(ptr, &heap)))
    {
        ft_putstr_fd("Realloc: invalid pointer\n", 2);
        return NULL;
    }
    // ?? below condition
    if (block->size - sizeof(t_block) <= size)
    {
        divide_block(&heap, &block);
        return block;
    }
    void *new_ptr;
    if ((new_ptr = try_filling_heap(&heap, &block, size)) || (new_ptr = try_adjacent_block(&heap, &block, size)))
        return new_ptr;
}