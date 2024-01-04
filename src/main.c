#include "../inc/malloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void print(char *s)
{
    write(1, s, strlen(s));
}

int main(void)
{
    int i;
    char *addr;

    i = 0;
    // while (i < 1024)
    // {
    addr = (char *)malloc(16);
    addr[15] = 0;
    for (int i = 0; i < 15; i++)
        addr[i] = '1';
    ptr_hex_dump(addr);
    // free(addr);
    // i++;
    // }
    // heap_info();
    return (0);
}