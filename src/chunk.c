#include "../inc/malloc.h"

void divide_chunk(t_heap **heap, t_chunk **divided_chunk, const size_t old_chunk_size)
{
    if (old_chunk_size - (*divided_chunk)->size >= (align_mem(1 + sizeof(t_chunk))))
    {
        // logger(BLOCK_DIVISION);
        t_chunk *new_chunk = (void *)(*divided_chunk) + (*divided_chunk)->size;
        new_chunk->prev = *divided_chunk;
        new_chunk->next = (*divided_chunk)->next;
        (*divided_chunk)->next = new_chunk;
        new_chunk->freed = true;
        new_chunk->size = old_chunk_size - (*divided_chunk)->size;
        (*heap)->chunk_count++;
    }
}

void *find_free_chunk(t_heap *heap, const size_t size)
{
    t_chunk *chunk = heap->chunk;
    while (chunk)
    {
        if (chunk->freed && chunk->size >= size + sizeof(t_chunk))
            return chunk;
        chunk = chunk->next;
    }
    ft_putstr_fd("Malloc: Error while attributing chunk\n", 2);
    return NULL;
}

// bool search_chunks(t_heap *heap, const size_t size)
// {
//     t_chunk *chunk = heap->chunk;
//     while (chunk)
//     {
//         if (chunk->freed && chunk->size >= size + sizeof(t_chunk))
//             return true;
//         chunk = chunk->next;
//     }
//     return false;
// }

t_chunk *check_freed_chunks(t_heap **heap, const size_t size)
{
    for (t_chunk *chunk = (*heap)->chunk; chunk; chunk = chunk->next)
    {
        if (chunk->freed && chunk->size >= size + sizeof(t_chunk))
        {
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
        (*heap)->chunk = (void *)(*heap) + sizeof(t_heap);
        (*heap)->chunk->next = NULL;
        (*heap)->chunk->prev = NULL;
        (*heap)->chunk->freed = false;
        (*heap)->chunk->size = size + sizeof(t_chunk);
        (*heap)->free_space -= (*heap)->chunk->size;
        return (*heap)->chunk;
    }

    t_chunk *last_chunk = get_last_chunk(heap);
    t_chunk *new_chunk = (void *)last_chunk + last_chunk->size;
    (*heap)->chunk_count++;
    last_chunk->next = new_chunk;
    new_chunk->next = NULL;
    new_chunk->prev = last_chunk;
    new_chunk->freed = false;
    new_chunk->size = size + sizeof(t_chunk);
    (*heap)->free_space -= new_chunk->size;
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
    // check_freed_chunks();
    // return ret_chunk;
    // if (search_chunks(*heap, size))
    // {
    //     t_chunk *found_chunk = find_free_chunk(*heap, size);
    //     // logger(BLOCK_ATTRIBUTION);
    //     size_t old_chunk_size = found_chunk->size;
    //     found_chunk->size = size + sizeof(t_chunk);
    //     found_chunk->freed = false;
    //     divide_chunk(heap, &found_chunk, old_chunk_size);
    //     return ((void *)found_chunk + sizeof(t_chunk));
    // }

    if (check_heap_free_space(*heap, size))
    {
        ret_chunk = addback_new_chunk(heap, size);
        return ((void *)ret_chunk + sizeof(t_chunk));
        // if ((*heap)->chunk_count == 0)
        // {
        //     (*heap)->chunk_count++;
        //     (*heap)->chunk = (void *)(*heap) + sizeof(t_heap);
        //     (*heap)->chunk->next = NULL;
        //     (*heap)->chunk->prev = NULL;
        //     (*heap)->chunk->freed = false;
        //     (*heap)->chunk->size = size + sizeof(t_chunk);
        //     (*heap)->free_space -= (*heap)->chunk->size;
        //     // logger(BLOCK_CREATION);
        //     return ((void *)(*heap)->chunk + sizeof(t_chunk));
        // }

        // t_chunk *lst = (*heap)->chunk, *last_chunk = lst;
        // while (lst)
        // {
        //     last_chunk = lst;
        //     lst = lst->next;
        // }
        // t_chunk *new_chunk = (void *)last_chunk + last_chunk->size;

        // (*heap)->chunk_count++;
        // last_chunk->next = new_chunk;
        // new_chunk->next = NULL;
        // new_chunk->prev = last_chunk;
        // new_chunk->freed = false;
        // new_chunk->size = size + sizeof(t_chunk);
        // (*heap)->free_space -= new_chunk->size;
        // // logger(BLOCK_CREATION);
        // return ((void *)new_chunk + sizeof(t_chunk));
    }
    return NULL;
}