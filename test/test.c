#include "../inc/malloc.h"
#include <string.h> // strcmp()

static void basic_test(void)
{
    ft_putstr_fd("Basic test...\n", 1);
    void *ptr0_tiny = malloc(1);
    void *ptr1_tiny = malloc(1);
    heap_info();
    free(ptr0_tiny);
    free(ptr1_tiny);
    heap_info();
    void *ptr0_small = malloc(100);
    void *ptr1_small = malloc(100);
    free(ptr0_small);
    heap_info();
    free(ptr1_small);
    void *ptr0_large = malloc(100000);
    void *ptr1_large = malloc(100000);
    heap_info();
    free(ptr0_large);
    free(ptr1_large);
    heap_info();
}

static void realloc_test(void)
{
    ft_putstr_fd("Realloc test...\n", 1);
    //
}

static void advanced_test(void)
{
    ft_putstr_fd("Advanced test...\n", 1);
    //
}

static void defrag_test(void)
{
    ft_putstr_fd("Defrag test...\n", 1);
    //
}

static void ptr_hex_dump_test(void)
{
    ft_putstr_fd("Ptr hex dump test...\n", 1);
    //
}

int main(int argc, char **argv)
{
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "basic") == 0))
        basic_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "realloc") == 0))
        realloc_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "advanced") == 0))
        advanced_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "defrag") == 0))
        defrag_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "hexdump") == 0))
        ptr_hex_dump_test();

    return 0;
}