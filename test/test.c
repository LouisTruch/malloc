#include "../inc/malloc.h"
#include <string.h> // strcmp()

static void print_category(char *category)
{
    for (size_t i = 0; i < 50 - strlen(category); i++)
        ft_putstr_fd("-", 1);
    ft_putstr_fd(category, 1);
    for (size_t i = 0; i < 75 - strlen(category); i++)
        ft_putstr_fd("-", 1);
    ft_putstr_fd("\n", 1);
}

static void basic_test(void)
{
    ft_putstr_fd("Basic test...\n", 1);
    print_category("Tiny basic test");
    void *ptr0_tiny = malloc(1);
    void *ptr1_tiny = malloc(1);
    void *ptr2_tiny = malloc(1);
    free(ptr0_tiny);
    free(ptr2_tiny);
    heap_info();
    free(ptr1_tiny);
    heap_info();

    print_category("Small basic test");
    void *ptr0_small = malloc(100);
    char *ptr1_small = malloc(100);
    free(ptr0_small);
    heap_info();
    free(ptr1_small);
    heap_info();

    print_category("Large basic test");
    void *ptr0_large = malloc(100000);
    void *ptr1_large = malloc(100000);
    heap_info();
    free(ptr0_large);
    free(ptr1_large);
    heap_info();

    print_category("Random basic test");
    ptr0_tiny = malloc(1);
    ptr0_small = malloc(100);
    ptr0_large = malloc(10000);
    heap_info();
    free(ptr0_tiny);
    free(ptr0_small);
    free(ptr0_large);
    heap_info();
}

static void overhead_test(void)
{
    ft_putstr_fd("Overhead test...\n", 1);
    int i;
    char *addr;

    i = 0;
    while (i < 1024)
    {
        addr = (char *)malloc(1024);
        if (addr == NULL)
        {
            ft_putstr_fd("Failed to allocate memory\n", 2);
            return;
        }
        addr[0] = 42;
        i++;
    }
}

static void page_caching_test(void)
{
    ft_putstr_fd("Page caching test...\n", 1);
    int i;
    char *addr;

    i = 0;
    while (i < 10240)
    {
        addr = (char *)malloc(1024);
        (void)addr;
        free(addr);
        i++;
    }
    // heap_info();
}

#define M (1024 * 1024)

static void realloc_test(void)
{
    ft_putstr_fd("Realloc test...\n", 1);
    char *addr1;
    char *addr2;
    char *addr3;

    addr1 = (char *)malloc(16 * M);
    if (addr1 == NULL)
    {
        ft_putstr_fd("Failed to allocate memory\n", 2);
        return;
    }
    strcpy(addr1, "Hello world!\n");
    ft_putstr_fd(addr1, 1);
    addr2 = (char *)malloc(16 * M);
    if (addr2 == NULL)
    {
        ft_putstr_fd("Failed to allocate memory\n", 2);
        return;
    }
    addr3 = (char *)realloc(addr1, 128 * M);
    if (addr3 == NULL)
    {
        ft_putstr_fd("Failed to reallocate memory\n", 2);
        return;
    }
    addr3[127 * M] = 42;
    ft_putstr_fd(addr3, 1);
}

static void show_alloc_mem_test(void)
{
    ft_putstr_fd("Show alloc mem test...\n", 1);
    void *a = malloc(1);
    (void)a;
    void *b = malloc(2);
    (void)b;
    void *c = malloc(4);
    (void)c;
    void *d = malloc(8);
    (void)d;
    void *e = malloc(16);
    (void)e;
    void *f = malloc(32);
    (void)f;
    void *g = malloc(64);
    (void)g;
    void *h = malloc(128);
    (void)h;
    void *i = malloc(256);
    (void)i;
    void *j = malloc(512);
    (void)j;
    void *k = malloc(1024);
    (void)k;
    void *l = malloc(1024 * 2);
    (void)l;
    void *m = malloc(1024 * 4);
    (void)m;
    void *n = malloc(1024 * 32);
    (void)n;
    void *o = malloc(M);
    (void)o;
    void *p = malloc(16 * M);
    (void)p;
    void *q = malloc(128 * M);
    (void)q;
    show_alloc_mem();
}

static void alignement_test(void)
{
    ft_putstr_fd("Alignement test...\n", 1);
    int i;
    int alignment;
    char *addr;

    i = 1;
    alignment = 2 * sizeof(size_t);
    while (i <= 100)
    {
        addr = (char *)malloc(i * i * i);
        if (addr == NULL)
        {
            ft_putstr_fd("Failed to allocate memory\n", 2);
            return;
        }
        if ((((unsigned long)(addr)) % alignment) != 0)
        {
            ft_putstr_fd("malloc returned a non aligned boundary\n", 2);
            return;
        }
        i++;
        free(addr);
    }
}

static void advanced_test(void)
{
    ft_putstr_fd("Advanced test...\n", 1);
    //
}

static void defrag_test(void)
{
    ft_putstr_fd("Defrag test...\n", 1);
    char *p0 = malloc(1);
    char *p1 = malloc(1);
    char *p2 = malloc(1);
    heap_info();
    free(p0);
    heap_info();
    free(p1);
    heap_info();
    free(p2);
    heap_info();
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
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "overhead") == 0))
        overhead_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "cache") == 0))
        page_caching_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "realloc") == 0))
        realloc_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "showalloc") == 0))
        show_alloc_mem_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "align") == 0))
        alignement_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "advanced") == 0))
        advanced_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "defrag") == 0))
        defrag_test();
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "hexdump") == 0))
        ptr_hex_dump_test();

    return 0;
};