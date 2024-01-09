#include "../inc/malloc.h"

void divide_chunk(t_heap **heap, t_chunk **divided_chunk, const size_t old_chunk_size)
{
    if (old_chunk_size - (*divided_chunk)->size >= (align_mem(1 + sizeof(t_chunk))))
    {
        logger(CHUNK_DIVISION, &divided_chunk);
        t_chunk *new_chunk = (void *)(*divided_chunk) + (*divided_chunk)->size;
        new_chunk->prev = *divided_chunk;
        new_chunk->next = (*divided_chunk)->next;
        (*divided_chunk)->next = new_chunk;
        new_chunk->freed = true;
        new_chunk->size = old_chunk_size - (*divided_chunk)->size;
        (*heap)->chunk_count++;
    }
}

t_chunk *check_freed_chunks(t_heap **heap, const size_t size)
{
    for (t_chunk *chunk = (*heap)->chunk; chunk; chunk = chunk->next)
    {
        if (chunk->freed && chunk->size >= size)
        {
            logger(CHUNK_ATTRIBUTION, &heap);
            return chunk;
        }
    }
    return NULL;
}

static t_chunk *get_last_chunk(t_heap **heap)
{
    t_chunk *chunk = (*heap)->chunk;
    while (chunk->next)
        chunk = chunk->next;
    return chunk;
}

t_chunk *addback_new_chunk(t_heap **heap, const size_t size)
{
    if (!(*heap)->chunk)
    {
        (*heap)->chunk_count++;
        (*heap)->chunk = (void *)(*heap) + sizeof(t_heap) + HEAP_SHIFT;
        (*heap)->chunk->next = NULL;
        (*heap)->chunk->prev = NULL;
        (*heap)->chunk->freed = false;
        (*heap)->chunk->size = size;
        (*heap)->free_space -= (*heap)->chunk->size;
        logger(CHUNK_CREATION, (*heap)->chunk);
        return (*heap)->chunk;
    }
    t_chunk *last_chunk = get_last_chunk(heap);
    t_chunk *new_chunk = (void *)last_chunk + last_chunk->size;
    (*heap)->chunk_count++;
    last_chunk->next = new_chunk;
    new_chunk->next = NULL;
    new_chunk->prev = last_chunk;
    new_chunk->freed = false;
    new_chunk->size = size;
    (*heap)->free_space -= new_chunk->size;
    logger(CHUNK_CREATION, new_chunk);
    return new_chunk;
}

void *create_chunk(t_heap **heap, const size_t size)
{
    if (!*heap)
        return NULL;
    t_chunk *ret_chunk = check_freed_chunks(heap, size);
    if (ret_chunk)
    {
        ret_chunk->freed = false;
        size_t old_chunk_size = ret_chunk->size;
        divide_chunk(heap, &ret_chunk, old_chunk_size);
        return ((void *)ret_chunk + sizeof(t_chunk));
    }
    if (check_heap_free_space(*heap, size))
    {
        ret_chunk = addback_new_chunk(heap, size);
        return ((void *)ret_chunk + sizeof(t_chunk));
    }
    return NULL;
}