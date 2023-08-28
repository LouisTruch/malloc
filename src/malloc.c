#include "../inc/malloc.h"

#include <stdio.h>
int main(void)
{
    printf("tiny:%li small:%li\n", TINY_ALLOC, SMALL_ALLOC);
}