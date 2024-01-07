#include "../inc/malloc.h"

static size_t print_blocks(t_chunk *chunk)
{
    size_t total_alloc_block = 0;
    while (chunk)
    {
        if (1)
        {
            ft_dprintf(1, "%p - %p : %i bytes freed %i\n", chunk, (void *)chunk + chunk->size, chunk->size,
                       chunk->freed);
            if (!chunk->freed)
                total_alloc_block += chunk->size;
        }
        chunk = chunk->next;
    }
    return total_alloc_block;
}

void print_heap_info(t_heap *heap)
{
    if (heap->total_size == TINY_ARENA)
        ft_putstr_fd("TINY", 1);
    else if (heap->total_size == SMALL_ARENA)
        ft_putstr_fd("SMALL", 1);
    else
        ft_putstr_fd("LARGE", 1);
    ft_dprintf(1, "%p - %p\n", heap, heap + heap->total_size);
}

void show_alloc_mem(void)
{
    t_heap *heap = g_heap;
    size_t total_alloc = 0;

    pthread_mutex_lock(&g_mutex);
    while (heap)
    {
        print_heap_info(heap);
        total_alloc += print_blocks(heap->chunk);
        heap = heap->next;
    }
    ft_dprintf(1, "Total : %i bytes\n", total_alloc);
    pthread_mutex_unlock(&g_mutex);
}