#include "../inc/malloc.h"

static void print_block_info(t_block *block)
{
    ft_printf("block address:%X block_size:%i freed:%i\n", block, block->size, block->freed);
}

static void print_heap_info(t_heap *heap)
{
    ft_printf("heap address:%X arena_size:%i free_space:%i, block_count:%i, first block address:%X\n", heap, heap->arena_size, heap->free_space, heap->block_count, heap->block);

    t_block *block = heap->block;
    while (block)
    {
        print_block_info(block);
        block = block->next;
    }
}

int main(void)
{
    ft_printf("size of t_heap:%i t_block:%i\n", sizeof(t_heap), sizeof(t_block));
    ft_printf("tiny arena=%i small arena=%i\n", TINY_ARENA, SMALL_ARENA);
    ft_printf("tiny alloc<=%i small alloc<=%i\n", TINY_ALLOC, SMALL_ALLOC);
    char *ptr = my_malloc(10);
    char *ptrr = my_malloc(10);
    char *ptrrr = my_malloc(10);
    void *ptr2 = my_malloc(300);
    void *ptr3 = my_malloc(55);
    void *ptr4 = my_malloc(1000);
    void *ptr5 = my_malloc(400);

    ft_strlcat(ptr, "slttest\n", 10);
    ft_printf(ptr);
    t_heap *heap = g_heap;
    while (heap)
    {
        print_heap_info(heap);
        heap = heap->next;
    }

    ft_printf("free--------------\n");
    my_free(ptr2);
    my_free(ptr5);
    my_free(ptr4);
    // ptr = my_malloc(55);

    t_heap *heap2 = g_heap;
    while (heap2)
    {
        print_heap_info(heap2);
        heap2 = heap2->next;
    }
}