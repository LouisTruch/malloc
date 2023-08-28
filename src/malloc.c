#include "../inc/malloc.h"

static int get_alloc_size(size_t size)
{
    if (size <= TINY_ALLOC)
    {
        return TINY;
    }
    else if (size > TINY_ALLOC && size <= SMALL_ALLOC)
    {
        return SMALL;
    }
    else
    {
        return LARGE;
    }
}

static t_block *check_heap_free_space(t_heap *heap, size_t size)
{
    if (heap->free_space >= size + sizeof(t_block))
    {
        // Create a new block and link it
    }
    return NULL;
}

static t_block *search_adapted_block(t_heap *heap, size_t size)
{
    while(heap->block != NULL)
    {
        if (heap->block->size >= size + sizeof(t_block) && heap->block->freed)
        {
            return heap->block;
        }
        heap->block = heap->block->next;
    }
    return NULL;
}

static t_heap *search_adapted_heap(int size)
{
    (void)size;
    size_t adapted_alloc_size = get_alloc_size(size);
    t_heap *heap = g_heap;
    while (heap != NULL)
    {
        if (heap->arena_size == adapted_alloc_size)
        {
            if (search_adapted_block(heap, size) || check_heap_free_space(heap, size))
            {
                return heap;
            }
        }
        heap = heap->next;
    }
    return NULL;
}

static t_heap *allocate_heap(size_t size, int arena_size)
{
    printf("Heap allocation of %zuB\n", size);
    t_heap *heap;
    heap = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g_heap == MAP_FAILED)
    {
        printf("mmap fail\n");
        // Handle mmap error here
        return NULL;
    }
    heap->arena_size = arena_size;
    heap->block_count = 0;
    heap->block = NULL;
    heap->prev = NULL;
    heap->next = NULL;
    heap->free_space = size - sizeof(t_heap);
    return heap;
}

static t_heap *initialize_heap(size_t arena_alloc_size, size_t size)
{
    switch (arena_alloc_size) {
        case TINY:
            return (allocate_heap(TINY_ARENA, TINY));
            break;
        case SMALL:
            return (allocate_heap(SMALL_ARENA, SMALL));
            break;
        case LARGE:
            return (allocate_heap(size + sizeof(t_heap), LARGE));
            break;
    }
}

void *my_malloc(size_t size)
{
    if (g_heap == NULL)
    {
        printf("Initiliaze heap\n");
        g_heap = initialize_heap(get_alloc_size(size), size);
    }

    t_heap *heap = search_adapted_heap(size);
    t_block *block = NULL;
    if (!heap)
    {
        // Allocate new heap
    }
    else
    {
        block = search_adapted_block(heap, size);
        if (!block)
            block = check_heap_free_space(heap, size);
    }
    // if (heap == NULL)
    // {
    //     // allocate new heap
    // }
    return ((void *)block + sizeof(t_block));
}