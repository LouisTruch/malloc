#include "../inc/malloc.h"
#include <sys/mman.h> // munmap()

void reset_chunk(t_heap **heap_to_dealloc)
{
    if (!(*heap_to_dealloc)->chunk)
        return;
    (*heap_to_dealloc)->free_space += (*heap_to_dealloc)->chunk->size;
    (*heap_to_dealloc)->chunk_count--;
    (*heap_to_dealloc)->chunk = NULL;
}

static void dealloc_heap(t_heap **heap)
{
    t_heap *heap_to_dealloc = *heap;
    if (heap_to_dealloc->heap_type != LARGE && check_if_last_heap_type(heap_to_dealloc->heap_type))
    {
        reset_chunk(&heap_to_dealloc);
        return;
    }

    if (!heap_to_dealloc->prev)
    {
        g_heap = heap_to_dealloc->next;
        if (!g_heap)
            goto munmap_call;
    }
    if (heap_to_dealloc->prev)
        heap_to_dealloc->prev->next = heap_to_dealloc->next;
    if (heap_to_dealloc->next)
        heap_to_dealloc->next->prev = heap_to_dealloc->prev;
munmap_call:
    logger(HEAP_DEALLOC, heap_to_dealloc);
    if (munmap(heap_to_dealloc, heap_to_dealloc->total_size))
        ft_putstr_fd("Free: munmap error\n", 2);
}

static void defrag_chunks(t_heap **heap, t_chunk **chunk)
{
    if ((*chunk)->next && (*chunk)->next->freed)
    {
        logger(MEM_DEFRAG, chunk);
        (*chunk)->size += (*chunk)->next->size;
        (*chunk)->next = (*chunk)->next->next;
        if ((*chunk)->next)
            (*chunk)->next->prev = (*chunk);
        (*heap)->chunk_count--;
    }
    if ((*chunk)->prev && (*chunk)->prev->freed)
        defrag_chunks(heap, &(*chunk)->prev);
}

static void free_tiny_small(t_heap **heap, t_chunk **chunk)
{
    logger(CHUNK_FREED, &chunk);
    (*chunk)->freed = true;
    if (((*chunk)->prev && (*chunk)->prev->freed) || ((*chunk)->next && (*chunk)->next->freed))
        defrag_chunks(heap, chunk);
    if ((*heap)->chunk_count == 1 && (*heap)->chunk->freed)
        dealloc_heap(heap);
}

void free(void *ptr)
{
    pthread_mutex_lock(&g_mutex);
    if (!ptr)
        goto end;

    // ft_dprintf(1, "%s0Free: %p%s\n", UWHITE, ptr, NC);
    ptr -= sizeof(t_chunk);
    logger(CALL_FREE, NULL);
    // ft_dprintf(1, "%s1Free: %p%s\n", UWHITE, ptr, NC);

    for (t_heap *current_heap = g_heap; current_heap; current_heap = current_heap->next)
    {
        for (t_chunk *current_chunk = current_heap->chunk; current_chunk; current_chunk = current_chunk->next)
        {
            if (current_chunk == ptr)
            {
                if (current_heap->heap_type == LARGE)
                    dealloc_heap(&current_heap);
                else
                    free_tiny_small(&current_heap, &current_chunk);
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