#include "../inc/malloc.h"
#include <sys/mman.h> // munmap()

static void dealloc_heap(t_heap **heap)
{
    // Need keep at least 1 page mapped theeeeeeeeeeeere instead of below
    t_heap *to_dealloc = *heap;
    if (!to_dealloc->prev)
    {
        g_heap = (*heap)->next;
        if (!g_heap)
            goto munmap_call;
    }
    if ((*heap)->prev)
        (*heap)->prev->next = (*heap)->next;
    if ((*heap)->next)
        (*heap)->next->prev = (*heap)->prev;
munmap_call:
    // Check that there is at least 1 heap
    if (munmap(to_dealloc, to_dealloc->total_size))
        ft_putstr_fd("Free: munmap error\n", 2);
    logger(HEAP_DEALLOC);
}

static void defrag_blocks(t_heap **heap, t_chunk **block)
{
    if ((*block)->next && (*block)->next->freed)
    {
        (*block)->size += (*block)->next->size;
        (*block)->next = (*block)->next->next;
        if ((*block)->next)
            (*block)->next->prev = (*block);
        (*heap)->chunk_count--;
        logger(MEM_DEFRAG);
    }
    if ((*block)->prev && (*block)->prev->freed)
        defrag_blocks(heap, &(*block)->prev);
}

static void free_tiny_small(t_heap **heap, t_chunk **block)
{
    (*block)->freed = true;
    logger(BLOCK_FREED);

    if (((*block)->prev && (*block)->prev->freed) || ((*block)->next && (*block)->next->freed))
        defrag_blocks(heap, block);
    if ((*heap)->chunk_count == 1 && (*heap)->chunk->freed)
        dealloc_heap(heap);
}

void free(void *ptr)
{
    pthread_mutex_lock(&g_mutex);
    if (!ptr)
        goto end;

    ptr -= sizeof(t_chunk);
    for (t_heap *current_heap = g_heap; current_heap; current_heap = current_heap->next)
    {
        // Can opti this cause of heap address' range
        // so we don't need to iter on every block of every heap
        for (t_chunk *current_block = current_heap->chunk; current_block; current_block = current_block->next)
        {
            if (current_block == ptr)
            {
                if (current_heap->total_size >= (size_t)SMALL_ARENA)
                    dealloc_heap(&current_heap);
                else
                    free_tiny_small(&current_heap, &current_block);
#ifdef HISTORY
                record_alloc_history(FREE, ptr);
#endif
                goto end;
            }
        }
    }
    ft_putstr_fd("Free: invalid pointer\n", 2);

end:
    pthread_mutex_unlock(&g_mutex);
}