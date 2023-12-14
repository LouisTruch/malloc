#include <stdlib.h>
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
    while (i < 10240)
    {
        addr = (char *)malloc(1024);
        free(addr);
        i++;
    }
    return (0);
}
