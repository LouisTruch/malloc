#include "../inc/malloc.h"

int main()
{
    for (int i = 0; i < 100; i++)
    {
        char *addr = malloc(10);
        addr[0] = 'z';
        free(addr);
    }

    return 0;
}