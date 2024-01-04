#include "../inc/malloc.h"
#include <sys/types.h>

static u_int8_t check_env_var()
{
    if (getenv(LOGGER_ENV_VAR))
    {
        ft_dprintf(1, "Logger Enabled\n");
        return ENABLED;
    }
    return DISABLED;
}

void logger(int action)
{
    static u_int8_t logger_state = UNDEFINED;
    if (logger_state == DISABLED)
        return;
    if (logger_state == UNDEFINED)
    {
        logger_state = check_env_var();
        if (logger_state == DISABLED)
            return;
    }

    switch (action)
    {
    case HEAP_ALLOC:
        ft_dprintf(1, "New Heap Allocation\n");
        break;
    case HEAP_DEALLOC:
        ft_dprintf(1, "Heap Deallocation\n");
        break;
    case BLOCK_CREATION:
        ft_dprintf(1, "New Block Created\n");
        break;
    case BLOCK_ATTRIBUTION:
        ft_dprintf(1, "Block Attributed\n");
        break;
    case BLOCK_FREED:
        ft_dprintf(1, "Block Freed\n");
        break;
    case BLOCK_DIVISION:
        ft_dprintf(1, "Block Divided\n");
        break;
    case MEM_DEFRAG:
        ft_dprintf(1, "Memory Defragmented\n");
        break;
    }
}