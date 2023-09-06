#include "../inc/malloc.h"

t_heap *g_heap = NULL;

static void *alloc_large(t_heap **heap, const size_t asked_size)
{
    t_heap *new_heap = mmap(NULL, asked_size + sizeof(t_heap) + sizeof(t_block), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_heap == MAP_FAILED)
    {
        // Swap fd 2
        ft_printf("Mmap failed\n");
        return NULL;
    }
    new_heap->arena_size = LARGE;
    new_heap->block = NULL;
    new_heap->block_count = 1;
    new_heap->free_space = 0;
    new_heap->block = (void *)new_heap + sizeof(t_heap);
    new_heap->block->freed = false;
    new_heap->block->size = asked_size + sizeof(t_block);
    if (!*heap)
    {
        new_heap->next = NULL;
        new_heap->prev = NULL;
        *heap = new_heap;
        return ((void *)new_heap + sizeof(t_heap) + sizeof(t_block));
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
    return ((void *)new_heap + sizeof(t_heap) + sizeof(t_block));
}

static bool search_free_space(t_heap *heap, const size_t asked_size)
{
    if (heap->free_space >= asked_size + sizeof(t_block))
        return true;
    return false;
}

static bool search_blocks(t_heap *heap, const size_t asked_size)
{
    return false;
}

static void *alloc_block(t_heap **heap, const size_t asked_size)
{
    if (search_blocks(*heap, asked_size))
    {
        return NULL;
    }
    if (search_free_space(*heap, asked_size))
    {
        if ((*heap)->block_count == 0)
        {
            (*heap)->block_count++;
            (*heap)->free_space -= sizeof(t_block) - asked_size;
            (*heap)->block = (void *)(*heap) + sizeof(t_heap);
            (*heap)->block->next = NULL;
            (*heap)->block->prev = NULL;
            (*heap)->block->freed = false;
            (*heap)->block->size = asked_size + sizeof(t_block);
            // ft_printf("heap:%X block:%X return:%X\n", *heap, (*heap)->block, (void *)(*heap)->block + sizeof(t_block));
            return ((void *)(*heap)->block + sizeof(t_block));
        }

        t_block *lst = (*heap)->block, *last_block = lst;
        while (lst)
        {
            last_block = lst;
            lst = lst->next;
        }
        t_block *new_block = (void *)last_block + sizeof(t_block) + last_block->size;
        (*heap)->block_count++;
        last_block->next = new_block;
        new_block->next = NULL;
        new_block->prev = last_block;
        new_block->freed = false;
        new_block->size = asked_size + sizeof(t_block);
        (*heap)->free_space -= new_block->size;
        // ft_printf("new_block_address:%X\n", new_block);
        return ((void *)new_block + sizeof(t_block));
    }
}

static void *alloc_tiny_small(t_heap **heap, const size_t arena_size, const size_t asked_size, const int arena_range)
{
    t_heap *lst = *heap, *last = NULL;
    while (lst)
    {
        if (lst->arena_size == arena_range && (search_blocks(lst, asked_size) || search_free_space(lst, asked_size)))
        {
            break;
        }
        last = lst;
        lst = lst->next;
    }
    if (!lst)
    {
        if ((lst = mmap(NULL, arena_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
        {
            // Swap to fd 2
            ft_printf("Mmap failed\n");
            return NULL;
        }
        if (last)
            last->next = lst;
        lst->prev = last;
        lst->next = NULL;
        lst->free_space = arena_size - sizeof(t_heap);
        lst->arena_size = get_arena_size(asked_size);
        lst->block_count = 0;
        lst->block = NULL;
        if (!*heap)
            *heap = lst;
    }
    return (alloc_block(&lst, asked_size));
}

void *my_malloc(size_t size)
{
    if (!size)
        return NULL;

    void *ptr = NULL;
    if (size <= TINY_ALLOC)
    {
        ptr = alloc_tiny_small(&g_heap, TINY_ARENA, size, TINY);
    }
    else if (size <= SMALL_ALLOC)
    {
        ptr = alloc_tiny_small(&g_heap, SMALL_ARENA, size, SMALL);
    }
    else
    {
        ptr = alloc_large(&g_heap, size);
    }
    return ptr;
    // ft_printf("Size asked %i\n", size);
    // debug_a(g_heap);

    // if (g_heap == NULL)
    //     g_heap = initialize_heap(get_alloc_range(size), size);

    // debug_a(g_heap);

    // t_heap *heap = search_adapted_heap(get_alloc_range(size), size);
    // if (heap == NULL)
    // {
    //     printf("heap=NULL\n");
    //     // New heap alloc
    //     // Allocate heap + link to existing linked list
    // }

    // void *ptr = get_block_address(heap, size);
    // int i = 0;
    // t_heap *test = g_heap;
    // while (test->block != NULL)
    // {
    //     ft_printf("%i\n", i);
    //     test->block = test->block->next;
    // }
    // debug_a(ptr);
    return ptr;
}

void linked_list(t_heap **heap)
{
    t_heap *test = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (!*heap)
    {
        *heap = test;
        return;
    }
    t_heap *head = *heap, *last = NULL;
    while (head)
    {
        last = head;
        head = head->next;
    }
    last->next = test;
}