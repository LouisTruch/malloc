#include "../inc/malloc.h"

int main(void)
{
    ft_printf("size of t_heap:%i t_block:%i\n", sizeof(t_heap), sizeof(t_block));
    // char* test = my_malloc(4);
    // char* test2 = my_malloc(8);
    // ft_strlcpy(test, "Slt", 4);
    // printf("Main: %s\n", test);
    my_malloc(10);
    my_malloc(2000);
    my_malloc(10);
    t_heap *heap = g_heap;
    while (heap)
    {
        printf("%X\n", heap);
        heap = heap->next;
    }
}