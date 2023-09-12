#include "../inc/malloc.h"

static void print_block_info(t_block *block)
{
    size_t test = block;
    bool aligned = test & 0xF;
    ft_printf("block address:%X block_size:%i freed:%i align:%X\n", block, block->size, block->freed, aligned);
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
    // char *ptr0 = my_malloc(35);
    // char *ptr4 = my_malloc(4);
    char *ptr1 = my_malloc(100);
    void *ptr2 = my_malloc(300);

    show_alloc_mem();
    my_free(ptr2);
    char *ptr3 = my_malloc(200);
    char *ptr4 = my_malloc(200);
    my_free(ptr4);
    my_free(ptr3);
    my_free(ptr2);
    my_free(ptr1);

    show_alloc_mem();
}