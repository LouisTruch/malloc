#include "../inc/malloc.h"

static bool check_free_space(t_heap *heap, size_t requested_size)
{
    return (heap->free_space >= requested_size + sizeof(t_block));
}

static bool check_adapted_block(t_block *block, size_t requested_size)
{
    while (block != NULL)
    {
        if (block->freed && block->size >= requested_size + sizeof(t_block))
            return true;
        block = block->next;
    }
    return false;
}

t_heap *search_adapted_heap(int arena_range, size_t requested_size)
{
    t_heap *heap = g_heap;

    while (heap != NULL)
    {
        if (heap->arena_size == arena_range && (check_adapted_block(heap->block, requested_size) || check_free_space(heap, requested_size)))
            return heap;
        heap = heap->next;
    }
    return NULL;
}

static t_heap *allocate_heap(size_t arena_size, int arena_range)
{
    logger(HEAP_ALLOC);

    t_heap *heap = mmap(NULL, arena_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (heap == MAP_FAILED)
    {
        // Replace to std error
        ft_printf("Mmap failed\n");
        return NULL;
    }
    heap->arena_size = arena_range;
    heap->next = NULL;
    heap->prev = NULL;
    heap->free_space = arena_size - sizeof(t_heap);
    heap->block = NULL;
    heap->block_count = 0;
    return heap;
}

t_heap *initialize_heap(int arena_range, size_t requested_size)
{
    switch (arena_range)
    {
    case TINY:
        return (allocate_heap(TINY_ARENA, TINY));
        break;
    case SMALL:
        return (allocate_heap(SMALL_ARENA, SMALL));
        break;
    case LARGE:
        return (allocate_heap(requested_size + sizeof(t_heap) + sizeof(t_block), LARGE));
        break;
    }
}