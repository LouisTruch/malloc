#include "../inc/malloc.h"
#include <sys/mman.h>     // mmap()
#include <sys/resource.h> // getrlimit()

t_heap *g_heap = NULL;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

// static void *alloc_large(t_heap **heap, const size_t asked_size)
// {
//     struct rlimit rlimit;
//     if (getrlimit(RLIMIT_AS, &rlimit) == -1)
//         return NULL;
//     if (asked_size + sizeof(t_heap) + sizeof(t_block) > rlimit.rlim_max)
//         return NULL;

//     t_heap *new_heap = mmap(NULL, asked_size + sizeof(t_heap) + sizeof(t_block), PROT_READ | PROT_WRITE,
//                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//     if (new_heap == MAP_FAILED)
//     {
//         ft_putstr_fd("Malloc: Mmap fail\n", 2);
//         return NULL;
//     }
//     ft_bzero(new_heap, sizeof(t_heap));
//     logger(HEAP_ALLOC);
//     new_heap->arena_size = LARGE;
//     new_heap->total_size = asked_size + sizeof(t_heap) + sizeof(t_block);
//     new_heap->block_count = 1;
//     new_heap->free_space = 0;
//     new_heap->block = (void *)new_heap + sizeof(t_heap) + HEAP_SHIFT;
//     new_heap->block->freed = false;
//     new_heap->block->size = asked_size + sizeof(t_block);
//     if (!*heap)
//     {
//         new_heap->next = NULL;
//         new_heap->prev = NULL;
//         *heap = new_heap;
//         logger(BLOCK_CREATION);
//         return ((void *)new_heap->block + sizeof(t_block));
//     }
//     t_heap *lst = *heap, *last = NULL;
//     while (lst)
//     {
//         last = lst;
//         lst = lst->next;
//     }
//     last->next = new_heap;
//     new_heap->prev = last;
//     new_heap->next = NULL;
//     logger(BLOCK_CREATION);
//     return ((void *)new_heap->block + sizeof(t_block));
// }

// static void *alloc_tiny_small(t_heap **heap, const size_t arena_size, const size_t size, const int arena_range)
// {
//     t_heap *lst_heap = *heap;
//     t_heap *last = NULL;
//     // t_heap *last == search_heap();
//     while (lst_heap)
//     {
//         if ((int)lst_heap->arena_size == arena_range &&
//             (search_blocks(lst_heap, size) || check_heap_free_space(lst_heap, size)))
//             break;
//         last = lst_heap;
//         lst_heap = lst_heap->next;
//     }
//     if (!lst_heap)
//     {
//         if ((lst_heap = mmap(NULL, arena_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) ==
//             MAP_FAILED)
//         {
//             ft_putstr_fd("Malloc: Mmap fail\n", 2);
//             return NULL;
//         }
//         ft_bzero(lst_heap, sizeof(t_heap));
//         logger(HEAP_ALLOC);
//         if (last)
//             last->next = lst_heap;
//         lst_heap->prev = last;
//         lst_heap->next = NULL;
//         lst_heap->total_size = arena_size;
//         lst_heap->free_space = arena_size - sizeof(t_heap);
//         lst_heap->arena_size = get_arena_size(size);
//         lst_heap->block_count = 0;
//         lst_heap->block = NULL;
//         if (!*heap)
//             *heap = lst_heap;
//     }
//     return (alloc_block(&lst_heap, size));
// }

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
    new_heap->chunk->size = size - sizeof(t_chunk) - sizeof(t_heap);
    logger(CHUNK_CREATION, new_heap->chunk);
    return (void *)(new_heap->chunk) + sizeof(t_chunk);
}

static void init_allocator(void)
{
    static bool alloc_initialized = false;
    if (!alloc_initialized)
    {
        addback_new_heap(TINY_ARENA, false);
        addback_new_heap(SMALL_ARENA, false);
        alloc_initialized = true;
    }
}

void *malloc(size_t size)
{
    pthread_mutex_lock(&g_mutex);
    logger(CALL_MALLOC, NULL);
    if (!size)
        return NULL;

    size = align_mem(size);
    void *ptr = NULL;
    init_allocator();
    if (size <= TINY_ALLOC)
        ptr = alloc_tiny_small(size, TINY_ARENA);
    else if (size <= SMALL_ALLOC)
        ptr = alloc_tiny_small(size, SMALL_ARENA);
    else
        ptr = alloc_large(size + sizeof(t_heap) + sizeof(t_chunk));

#ifdef HISTORY
    record_alloc_history(ALLOC, ptr);
#endif

    pthread_mutex_unlock(&g_mutex);
    return ptr;
}