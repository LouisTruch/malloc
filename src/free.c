#include "../inc/malloc.h"
#include <stdlib.h>   // getenv()
#include <sys/mman.h> // munmap()

static bool init_deallocator(void)
{
    static bool dealloc_initialized = false;
    static bool hold_mem = false;

    if (!dealloc_initialized)
    {
        dealloc_initialized = true;
        char *env_var = getenv(MEMORY_HOLD_ENV_VAR);
        if (env_var && !ft_strcmp(env_var, "true"))
            hold_mem = true;
    }
    return hold_mem;
}

static void reset_chunk(t_heap **heap_to_dealloc)
{
    if (!(*heap_to_dealloc)->chunk)
        return;
    (*heap_to_dealloc)->free_space += (*heap_to_dealloc)->chunk->size;
    (*heap_to_dealloc)->chunk_count--;
    (*heap_to_dealloc)->chunk = NULL;
}

static void dealloc_heap(t_heap **heap, const bool hold_mem)
{
    (void)hold_mem;
    if (hold_mem)
        return;
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

static void free_large(t_heap **heap, t_chunk **chunk, const bool hold_mem)
{
    logger(CHUNK_FREED, &chunk);
    (*chunk)->freed = true;
    dealloc_heap(heap, hold_mem);
}

static void free_tiny_small(t_heap **heap, t_chunk **chunk, const bool hold_mem)
{
    logger(CHUNK_FREED, &chunk);
    (*chunk)->freed = true;
    if (((*chunk)->prev && (*chunk)->prev->freed) || ((*chunk)->next && (*chunk)->next->freed))
        defrag_chunks(heap, chunk);
    if ((*heap)->chunk_count == 1 && (*heap)->chunk->freed)
        dealloc_heap(heap, hold_mem);
}

void free(void *ptr)
{
    pthread_mutex_lock(&g_mutex);
    const bool hold_mem = init_deallocator();
    logger(CALL_FREE, NULL);
    if (!ptr)
        goto end;

    ptr -= sizeof(t_chunk);
    for (t_heap *current_heap = g_heap; current_heap; current_heap = current_heap->next)
    {
        for (t_chunk *current_chunk = current_heap->chunk; current_chunk; current_chunk = current_chunk->next)
        {
            if (current_chunk == ptr)
            {
                if (current_heap->heap_type == LARGE)
                    free_large(&current_heap, &current_chunk, hold_mem);
                else
                    free_tiny_small(&current_heap, &current_chunk, hold_mem);
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