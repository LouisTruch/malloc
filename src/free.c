#include "../inc/malloc.h"

static void dealloc_heap(t_heap **heap)
{
    t_heap *to_dealloc = *heap;
    if (!to_dealloc->prev)
    {
        g_heap = (*heap)->next;
        if (!g_heap)
            goto munmap_call;
        // return;
    }
    (*heap)->prev->next = (*heap)->next;
    if ((*heap)->next)
        (*heap)->next->prev = (*heap)->prev;
munmap_call:
    if (munmap(to_dealloc, to_dealloc->total_size))
        ft_putstr_fd("Free: munmap error\n", 2);
    logger(HEAP_DEALLOC);
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
        logger(MEM_DEFRAG);
    }
    if ((*block)->prev && (*block)->prev->freed)
        defrag_blocks(heap, &(*block)->prev);
}

static void free_tiny_small(t_heap **heap, t_block **block)
{
    (*block)->freed = true;
    logger(BLOCK_FREED);

    if (((*block)->prev && (*block)->prev->freed) || ((*block)->next && (*block)->next->freed))
        defrag_blocks(heap, block);
    if ((*heap)->block_count == 1 && (*heap)->block->freed)
        dealloc_heap(heap);
}

void free(void *ptr)
{
    if (!ptr)
        return;

    ptr -= sizeof(t_block);

    t_heap *heap = g_heap;
    t_block *block = NULL;
    pthread_mutex_lock(&g_mutex);
    for (t_heap *current_heap = g_heap; current_heap; current_heap = current_heap->next)
    {
        // Can opti this cause of heap address' range
        // so we don't need to iter on every block of every heap
        block = current_heap->block;
        for (t_block *current_block = heap->block; current_block; current_block = current_block->next)
        {
            if (block == ptr)
            {
                if (current_heap->arena_size == LARGE)
                    dealloc_heap(&current_heap);
                else
                    free_tiny_small(&current_heap, &block);
                pthread_mutex_unlock(&g_mutex);
#ifdef HISTORY
                record_alloc_history(FREE, ptr);
#endif
                return;
            }
        }
    }

    pthread_mutex_unlock(&g_mutex);
    ft_putstr_fd("Free: invalid pointer\n", 2);
}