#include "../inc/malloc.h"
#include <stdlib.h> // getenv()

t_heap *g_heap = NULL;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void init_allocator(void)
{
    static bool alloc_initialized = false;
    if (!alloc_initialized)
    {
        alloc_initialized = true;
        for (size_t i = 0; i < HEAP_PER_TYPE_CACHED_DEFAULT; i++)
        {
            addback_new_heap(TINY_ARENA, false);
            addback_new_heap(SMALL_ARENA, false);
        }
    }
}

static void *alloc_tiny_small(const size_t size, const size_t arena_size)
{
    t_heap *heap = search_heap(size, arena_size);
    if (!heap)
        heap = addback_new_heap(arena_size, false);
    return (create_chunk(&heap, size));
}

static void *alloc_large(const size_t size)
{
    t_heap *new_heap = addback_new_heap(size, true);
    if (!new_heap)
        return NULL;
    new_heap->chunk_count = 1;
    new_heap->free_space = 0;
    new_heap->chunk = (void *)new_heap + sizeof(t_heap) + HEAP_SHIFT;
    new_heap->chunk->freed = false;
    new_heap->chunk->size = size - sizeof(t_heap);
    logger(CHUNK_CREATION, new_heap->chunk);
    return (void *)(new_heap->chunk) + sizeof(t_chunk);
}

void *malloc(size_t size)
{
    pthread_mutex_lock(&g_mutex);
    init_allocator();
    logger(CALL_MALLOC, NULL);
    void *ptr = NULL;
    if (!size)
        goto end;

    size = align_mem(size);
    size += sizeof(t_chunk);
    if (size <= TINY_ALLOC)
        ptr = alloc_tiny_small(size, TINY_ARENA);
    else if (size <= SMALL_ALLOC)
        ptr = alloc_tiny_small(size, SMALL_ARENA);
    else
        ptr = alloc_large(size + sizeof(t_heap));

#ifdef HISTORY
    record_alloc_history(ALLOC, ptr);
#endif

end:
    pthread_mutex_unlock(&g_mutex);
    return ptr;
}