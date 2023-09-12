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

static void *check_available_size(t_heap **heap, t_block **block, const size_t new_size)
{
    // If still enough room in the heap, grow the block
    if (!(*block)->next && ((*block)->size + (*heap)->free_space >= new_size + sizeof(t_block)))
    {
        (*heap)->free_space += (*block)->size;
        (*block)->size = new_size + sizeof(t_block);
        (*heap)->free_space -= (*block)->size;
        return (*block);
    }
    // If an adjacent block is freed and there is enough space
    // void *ptr = my_malloc(new_size);
    // my_free(*block);
    // return ptr;
    return NULL;
}

static void *try_filling_heap(t_heap **heap, t_block **block, const size_t new_size)
{
    if (!(*block)->next && (new_size - (*block)->size >= (*heap)->free_space))
    {
    }
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
    void *new_ptr;
    // ?? below condition
    if (block->size == size - sizeof(t_block))
    {
        divide_block(&heap, &block);
    }
    try_filling_heap();
}