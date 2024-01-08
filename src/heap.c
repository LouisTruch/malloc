#include "../inc/malloc.h"
#include <sys/mman.h>     // mmap()
#include <sys/resource.h> // getrlimit()

t_heap *addback_new_heap(const size_t heap_size, const bool is_large)
{
    t_heap *new_heap = allocate_new_heap(heap_size);
    if (!new_heap)
        return NULL;
    ft_bzero(new_heap, sizeof(t_heap));
    t_heap *last_heap = get_last_heap();
    if (last_heap)
        last_heap->next = new_heap;
    new_heap->prev = last_heap;
    new_heap->next = NULL;
    if (is_large)
        new_heap->heap_type = LARGE;
    else if (heap_size == TINY_ARENA)
        new_heap->heap_type = TINY;
    else if (heap_size == SMALL_ARENA)
        new_heap->heap_type = SMALL;
    new_heap->total_size = heap_size;
    new_heap->free_space = heap_size - sizeof(t_heap);
    new_heap->chunk_count = 0;
    new_heap->chunk = NULL;
    if (!g_heap)
        g_heap = new_heap;
    return new_heap;
}

t_heap *allocate_new_heap(const size_t heap_size)
{
    struct rlimit rlimit;
    if (getrlimit(RLIMIT_AS, &rlimit) == -1)
    {
        ft_putstr_fd("Malloc: getrlimit() failed\n", 2);
        return NULL;
    }
    if (heap_size >= rlimit.rlim_max)
    {
        ft_putstr_fd("Malloc: rlimit.rlim_max reached\n", 2);
        return NULL;
    }

    t_heap *new_heap = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_heap == MAP_FAILED)
    {
        ft_putstr_fd("Malloc: mmap() failed\n", 2);
        return NULL;
    }
    logger(HEAP_ALLOC, new_heap);
    return new_heap;
}

t_heap *get_last_heap(void)
{
    for (t_heap *heap = g_heap; heap; heap = heap->next)
    {
        if (!heap->next)
            return heap;
    }
    return NULL;
}

t_heap *search_heap(const size_t size, const size_t heap_size)
{
    t_heap *heap = g_heap;
    for (; heap; heap = heap->next)
    {
        if (heap->total_size == heap_size && (check_freed_chunks(&heap, size) || check_heap_free_space(heap, size)))
            return heap;
    }
    return heap;
}

bool check_heap_free_space(t_heap *heap, const size_t size)
{
    return (heap->free_space >= size + sizeof(t_chunk));
}

bool check_if_last_heap_type(const t_heap_type heap_type)
{
    size_t nb_heap_same_type = 0;
    for (t_heap *heap = g_heap; heap; heap = heap->next)
    {
        if (heap->heap_type == heap_type)
        {
            nb_heap_same_type++;
            if (nb_heap_same_type > HEAP_PER_TYPE_CACHED_DEFAULT)
                return false;
        }
    }
    return true;
}