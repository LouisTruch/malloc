#include "../inc/malloc.h"

t_heap *g_heap = NULL;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *alloc_large(t_heap **heap, const size_t asked_size)
{
    t_heap *new_heap = mmap(NULL, asked_size + sizeof(t_heap) + sizeof(t_block), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_heap == MAP_FAILED)
    {
        ft_putstr_fd("Malloc: Mmap fail\n", 2);
        return NULL;
    }
    logger(HEAP_ALLOC);
    new_heap->arena_size = LARGE;
    new_heap->total_size = asked_size + sizeof(t_heap) + sizeof(t_block);
    new_heap->block_count = 1;
    new_heap->free_space = 0;
    new_heap->block = (void *)new_heap + sizeof(t_heap) + HEAP_SHIFT;
    new_heap->block->freed = false;
    new_heap->block->size = asked_size + sizeof(t_block);
    if (!*heap)
    {
        new_heap->next = NULL;
        new_heap->prev = NULL;
        *heap = new_heap;
        logger(BLOCK_CREATION);
        return ((void *)new_heap->block + sizeof(t_block));
    }
    t_heap *lst = *heap, *last = NULL;
    while (lst)
    {
        last = lst;
        lst = lst->next;
    }
    last->next = new_heap;
    new_heap->prev = last;
    new_heap->next = NULL;
    logger(BLOCK_CREATION);
    return ((void *)new_heap + sizeof(t_heap) + sizeof(t_block));
}

static bool search_free_space(t_heap *heap, const size_t asked_size)
{
    return (heap->free_space >= asked_size + sizeof(t_block));
}

static bool search_blocks(t_heap *heap, const size_t asked_size)
{
    t_block *block = heap->block;
    while (block)
    {
        if (block->freed && block->size >= asked_size + sizeof(t_block))
            return true;
        block = block->next;
    }
    return false;
}

static void divide_block(t_heap **heap, t_block **found_block, const size_t old_block_size)
{
    if (old_block_size - (*found_block)->size >= (align_mem(1 + sizeof(t_block))))
    {
        t_block *new_block = (void *)(*found_block) + (*found_block)->size;
        new_block->prev = *found_block;
        new_block->next = (*found_block)->next;
        (*found_block)->next = new_block;
        new_block->freed = true;
        new_block->size = old_block_size - (*found_block)->size;
        (*heap)->block_count++;
        logger(BLOCK_DIVISION);
    }
}

static void *find_free_block(t_heap *heap, const size_t asked_size)
{
    t_block *block = heap->block;
    while (block)
    {
        if (block->freed && block->size >= asked_size + sizeof(t_block))
            return block;
        block = block->next;
    }
    ft_putstr_fd("Malloc: Error while attributing block\n", 2);
    return NULL;
}

static void *alloc_block(t_heap **heap, const size_t asked_size)
{
    if (search_blocks(*heap, asked_size))
    {
        t_block *found_block = find_free_block(*heap, asked_size);
        logger(BLOCK_ATTRIBUTION);
        size_t old_block_size = found_block->size;
        found_block->size = asked_size + sizeof(t_block);
        found_block->freed = false;
        divide_block(heap, &found_block, old_block_size);
        return ((void *)found_block + sizeof(t_block));
    }

    if (search_free_space(*heap, asked_size))
    {
        if ((*heap)->block_count == 0)
        {
            (*heap)->block_count++;
            (*heap)->block = (void *)(*heap) + sizeof(t_heap) + HEAP_SHIFT;
            (*heap)->block->next = NULL;
            (*heap)->block->prev = NULL;
            (*heap)->block->freed = false;
            (*heap)->block->size = asked_size + sizeof(t_block);
            (*heap)->free_space -= (*heap)->block->size;
            logger(BLOCK_CREATION);
            return ((void *)(*heap)->block + sizeof(t_block));
        }

        t_block *lst = (*heap)->block, *last_block = lst;
        while (lst)
        {
            last_block = lst;
            lst = lst->next;
        }
        t_block *new_block = (void *)last_block + last_block->size;

        (*heap)->block_count++;
        last_block->next = new_block;
        new_block->next = NULL;
        new_block->prev = last_block;
        new_block->freed = false;
        new_block->size = asked_size + sizeof(t_block);
        (*heap)->free_space -= new_block->size;
        logger(BLOCK_CREATION);
        return ((void *)new_block + sizeof(t_block));
    }
    return NULL;
}

static void *alloc_tiny_small(t_heap **heap, const size_t arena_size, const size_t asked_size, const int arena_range)
{
    t_heap *lst_heap = *heap;
    t_heap *last = NULL;
    while (lst_heap)
    {
        if ((int)lst_heap->arena_size == arena_range && (search_blocks(lst_heap, asked_size) || search_free_space(lst_heap, asked_size)))
            break;
        last = lst_heap;
        lst_heap = lst_heap->next;
    }
    if (!lst_heap)
    {
        if ((lst_heap = mmap(NULL, arena_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
        {
            ft_putstr_fd("Malloc: Mmap fail\n", 2);
            return NULL;
        }
        logger(HEAP_ALLOC);
        if (last)
            last->next = lst_heap;
        lst_heap->prev = last;
        lst_heap->next = NULL;
        lst_heap->total_size = arena_size;
        lst_heap->free_space = arena_size - sizeof(t_heap);
        lst_heap->arena_size = get_arena_size(asked_size);
        lst_heap->block_count = 0;
        lst_heap->block = NULL;
        if (!*heap)
            *heap = lst_heap;
    }
    return (alloc_block(&lst_heap, asked_size));
}

void *malloc(size_t size)
{
    if (!size)
        return NULL;

    size = align_mem(size);
    void *ptr = NULL;
    pthread_mutex_lock(&g_mutex);
    if (size <= TINY_ALLOC)
        ptr = alloc_tiny_small(&g_heap, TINY_ARENA, size, TINY);
    else if (size <= SMALL_ALLOC)
        ptr = alloc_tiny_small(&g_heap, SMALL_ARENA, size, SMALL);
    else
        ptr = alloc_large(&g_heap, size);

#ifdef HISTORY
    record_alloc_history(ALLOC, ptr);
#endif

    pthread_mutex_unlock(&g_mutex);
    return ptr;
}