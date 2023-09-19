#include "../inc/malloc.h"

static size_t print_blocks(t_block *block)
{
    size_t total_alloc_block = 0;
    while (block)
    {
        if (1)
        {
            ft_printf("%p - %p : %i bytes freed %i\n", block, (void *)block + block->size, block->size, block->freed);
            total_alloc_block += block->size;
        }
        block = block->next;
    }
    return total_alloc_block;
}

void print_heap_info(t_heap *heap)
{
    switch (heap->arena_size)
    {
    case TINY:
        ft_printf("TINY : ");
        break;

    case SMALL:
        ft_printf("SMALL : ");
        break;

    case LARGE:
        ft_printf("LARGE : ");
        break;
    }
    ft_printf("%p - %p\n", heap, heap + heap->total_size);
}

void show_alloc_mem(void)
{
    t_heap *heap = g_heap;

    size_t total_alloc = 0;
    while (heap)
    {
        print_heap_info(heap);
        total_alloc += print_blocks(heap->block);
        heap = heap->next;
    }
    ft_printf("Total : %i bytes\n", total_alloc);
}