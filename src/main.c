#include "../inc/malloc.h"

int main()
{
    char *addr = malloc(10);
    (void)addr;
    show_alloc_mem_hex();

    return 0;
}