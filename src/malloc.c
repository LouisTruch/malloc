#include "../inc/malloc.h"

t_heap *g_heap = NULL;

static void *alloc_large(t_heap **heap, const size_t asked_size)
{
    t_heap *new_heap = mmap(NULL, asked_size + sizeof(t_heap), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
    if (!*heap)
    {
        new_heap->next = NULL;
        new_heap->prev = NULL;
        *heap = new_heap;
        return ((void *)new_heap + sizeof(t_heap));
    }
    t_heap *lst = *heap, *last = NULL;
    while(lst)
    {
        last = lst;
        lst = lst->next;
    }
    last->next = new_heap;
    new_heap->prev = last;
    new_heap->next = NULL;
    return ((void *)new_heap + sizeof(t_heap));
}

static bool search_free_space(t_heap *heap, const size_t asked_size)
{
    return false;
}

static bool search_blocks(t_heap *heap, const size_t asked_size)
{
    return false;
}

static void *alloc_tiny_small(t_heap **heap, const size_t arena_size, const size_t asked_size)
{
    t_heap *lst = *heap, *last = NULL;
    while(lst)
    {
        if (search_blocks(lst, asked_size) || search_free_space(lst, asked_size))
        {
            break ;
        }
        last = lst;
        lst = lst->next;
    }
    if (!lst)
    {
        if ((lst = mmap(NULL, arena_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
        {
            //Swap to fd 2
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
    return NULL;
}

void *my_malloc(size_t size)
{
    if (!size)
        return NULL;

    void *ptr = NULL;
    if (size <= TINY_ALLOC)
    {
        ptr = alloc_tiny_small(&g_heap, TINY_ARENA, size);
    }
    else if (size <= SMALL_ALLOC)
    {
        ptr = alloc_tiny_small(&g_heap, SMALL_ARENA, size);
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
        return ;
    }
    t_heap *head = *heap, *last = NULL;
    while (head)
    {
        last = head;
        head = head->next;
    }
    last->next = test;
}