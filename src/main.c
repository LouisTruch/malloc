#include "../inc/malloc.h"

int main(void)
{
    ft_printf("size of t_heap:%i t_block:%i\n", sizeof(t_heap), sizeof(t_block));
    ft_printf("tiny arena=%i small arena=%i\n", TINY_ARENA, SMALL_ARENA);
    ft_printf("tiny alloc<=%i small alloc<=%i\n", TINY_ALLOC, SMALL_ALLOC);

    char *p = my_malloc(10);
    p[0] = 'x';
    ft_printf("le p: %s\n", p);
    show_alloc_mem();

    char *p2 = my_realloc(p, 5);
    ft_printf("le p2: %s\n", p2);
    show_alloc_mem();
}