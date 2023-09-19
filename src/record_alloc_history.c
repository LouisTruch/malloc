#include "../inc/malloc.h"

static void record_malloc(t_block *block, int fd)
{
    (void)block;
    ft_putstr_fd("Allocation: size: ", fd);
    ft_putstr_fd("10\n", fd);
}

static void record_free(t_block *block, int fd)
{
    (void)block;
    ft_putstr_fd("Free: ", fd);
}

void record_alloc_history(int function_call, void *ptr)
{
    if (!ptr)
    {
        ft_putstr_fd("Record alloc history error: NULL ptr\n", 2);
        return;
    }
    int fd = open(HISTORY_FILEPATH, O_WRONLY | O_APPEND | O_CREAT | O_EXCL, 0604);
    if (fd == -1 && (EEXIST == errno))
    {
        fd = open(HISTORY_FILEPATH, O_WRONLY | O_APPEND);
        if (fd == -1)
        {
            ft_putstr_fd("Error while opening alloc history file\n", 2);
            return;
        }
    }

    if (function_call == ALLOC)
        record_malloc((void *)ptr - sizeof(t_block), fd);
    else if (function_call == FREE)
        record_free((void *)ptr - sizeof(t_block), fd);

    close(fd);
}