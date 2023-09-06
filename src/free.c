#include "../inc/malloc.h"

static void dealloc_heap(t_heap **heap)
{
    t_heap *to_dealloc = *heap;
    if (!(*heap)->prev)
    {
        g_heap = (*heap)->next;
        g_heap->prev = NULL;
        return;
    }
    (*heap)->prev->next = (*heap)->next;
    if ((*heap)->next)
        (*heap)->next->prev = (*heap)->prev;
    int ret = munmap(to_dealloc, to_dealloc->total_size);
    if (ret)
    {
        // Swap to fd 2
        ft_printf("munmap error\n");
    }
}

static void defrag_blocks(t_heap **heap, t_block **block)
{
    if ((*block)->next && (*block)->next->freed)
    {
        (*block)->size += (*block)->next->size;
        (*block)->next = (*block)->next->next;
        if ((*block)->next)
            (*block)->next->prev = (*block);
        (*heap)->block_count--;
    }
    if ((*block)->prev && (*block)->prev->freed)
        defrag_blocks(heap, &(*block)->prev);
}

static void free_tiny_small(t_heap **heap, t_block **block)
{
    (*block)->freed = true;

    if (((*block)->prev && (*block)->prev->freed) || ((*block)->next && (*block)->next->freed))
    {
        defrag_blocks(heap, block);
    }
    if ((*heap)->block_count == 1 && (*heap)->block->freed)
    {
        ft_printf("unalloc heap\n");
        dealloc_heap(heap);
    }
}

void my_free(void *ptr)
{
    ptr -= sizeof(t_block);

    t_heap *heap = g_heap;
    t_block *block = NULL;
    while (heap)
    {
        // Can opti this cause of heap address' range
        // and don't need to iter on every block of every heap
        block = heap->block;
        while (block)
        {
            if (block == ptr)
            {
                if (heap->arena_size == LARGE)
                {
                    dealloc_heap(&heap);
                }
                else
                {
                    free_tiny_small(&heap, &block);
                }
                return;
            }
            block = block->next;
        }
        heap = heap->next;
    }
    // Swap to fd 2
    ft_printf("Invalid pointer\n");
}