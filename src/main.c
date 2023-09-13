#include "../inc/malloc.h"

int main(void)
{
    ft_printf("size of t_heap:%i t_block:%i\n", sizeof(t_heap), sizeof(t_block));
    ft_printf("tiny arena=%i small arena=%i\n", TINY_ARENA, SMALL_ARENA);
    ft_printf("tiny alloc<=%i small alloc<=%i\n", TINY_ALLOC, SMALL_ALLOC);
}