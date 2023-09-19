#include "../inc/malloc.h"

static void hex_dump_heap(const void *ptr, const size_t size)
{
    const unsigned char *pc = (const unsigned char *)ptr;

    for (size_t i = 0; i < size; i++)
    {
        ft_printf("%X  ", pc[i]);
        if (i != 0 && i % 64 == 0)
        {
            ft_printf("\n");
        }
    }
}

// Change to dump blocks and not the whole heap
void show_alloc_mem_hex(void)
{
    t_heap *heap = g_heap;
    pthread_mutex_lock(&g_mutex);
    while (heap)
    {
        print_heap_info(heap);
        hex_dump_heap(heap, heap->total_size);
        heap = heap->next;
    }
    pthread_mutex_unlock(&g_mutex);
}