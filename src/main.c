#include "../inc/malloc.h"

int main()
{
    int i = 0;
    char *addr;

    while (i < 1024)
    {
        addr = malloc(1024);
        addr[0] = 42;
        free(addr);
        i++;
    }
    return 0;
}