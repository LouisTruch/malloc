#include "../inc/malloc.h"

int main()
{
    char *test = malloc(10);
    test[0] = 2;
    void *tg = malloc(100001);
    // free(test);
    test = malloc(100000);
    heap_info();
    free(test);

    return 0;
}