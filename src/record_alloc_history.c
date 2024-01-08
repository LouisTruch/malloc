#include "../inc/malloc.h"

static void record_malloc(t_chunk *block, int fd)
{
    ft_dprintf(fd, "Allocation address: %X Size: %i\n", block, block->size);
}

static void record_free(t_chunk *block, int fd)
{
    // ft_dprintf(fd, "Allocation: Size: %i", block->size);
    // ft_putstr_fd("Free: \n", fd);
    ft_dprintf(fd, "Free address: %X\n", block);
}

// Must compile with -DHISTORY to enable this feature
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
        record_malloc((void *)ptr - sizeof(t_chunk), fd);
    else if (function_call == FREE)
        record_free((void *)ptr - sizeof(t_chunk), fd);

    close(fd);
}