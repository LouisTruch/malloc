#include "../inc/malloc.h"

int main()
{
    for (int i = 0; i < 1; i++)
    {
        char *addr = malloc(100);
        void *test = malloc(100000);
        // (void)test;
        addr[0] = 'z';
        // free(addr);
        free(test);
    }
    

    return 0;
}