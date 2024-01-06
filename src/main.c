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
    char *t1;
    char *t2;
    char *t3;

    i = 0;
    while (i < 12)
    {
        addr = (char *)malloc(16000);
        t1 = (char *)malloc(16000);
        t2 = malloc(2);
        t3 = malloc(500);
        t1[1] = 2;
        t2[0] = 48;
        addr[15] = 3;
        heap_info();
        free(addr);
        free(t1);
        free(t2);
        free(t3);
        i++;
    }
    heap_info();
    return (0);
}