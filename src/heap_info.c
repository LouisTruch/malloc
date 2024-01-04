#include "../inc/malloc.h"

void heap_info(void)
{
    static long nb_call = 0;
    long int total_alloc = 0;
    pthread_mutex_lock(&g_mutex);
    ft_dprintf(1, "%s%i Heap Info:%s\n", UWHITE, nb_call, NC);
    for (t_heap *current_heap = g_heap; current_heap; current_heap = current_heap->next)
    {
        ft_dprintf(1, "%sHeap:\t%p - %p %s", BRED, current_heap, current_heap + current_heap->total_size, NC);
        ft_dprintf(1, "%sArena:", BYELLOW);
        switch (current_heap->arena_size)
        {
        case TINY:
            ft_putstr_fd("TINY", 1);
            break;
        case SMALL:
            ft_putstr_fd("SMALL", 1);
            break;
        case LARGE:
            ft_putstr_fd("LARGE", 1);
            break;
        }
        ft_dprintf(1, "%s ", NC);
        ft_dprintf(1, "%sSize:%i%s ", BGREEN, current_heap->total_size, NC);
        ft_dprintf(1, "%sFreeSpace:%i%s ", BCYAN, current_heap->free_space, NC);
        ft_dprintf(1, "%sNbBlock:%i%s\n", BPURPLE, current_heap->block_count, NC);
        for (t_block *current_block = current_heap->block; current_block; current_block = current_block->next)
        {
            ft_dprintf(1, "%sBlock:\t%p%s ", RED, current_block, NC);
            ft_dprintf(1, "%sRealSize:%i%s ", YELLOW, current_block->size, NC);
            ft_dprintf(1, "%sAlignedSize:%i%s ", GREEN, current_block->size - sizeof(t_block), NC);
            ft_dprintf(1, "%sFreed:", CYAN);
            if (current_block->freed)
                ft_putstr_fd("yes", 1);
            else
                ft_putstr_fd("no", 1);
            ft_dprintf(1, "%s\n", NC);
            total_alloc += current_block->size;
        }
    }
    ft_dprintf(1, "Total: %i bytes\n", total_alloc);
    pthread_mutex_unlock(&g_mutex);
    nb_call++;
}