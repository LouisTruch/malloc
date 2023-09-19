#include "../inc/malloc.h"

static int check_env_var()
{
    if (getenv(LOGGER_ENV_VAR))
    {
        ft_printf("Logger Enabled\n");
        return ENABLED;
    }
    return DISABLED;
}

void logger(int action)
{
    static int logger_state;
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
        ft_printf("New Heap Allocation\n");
        break;
    case HEAP_DEALLOC:
        ft_printf("Heap Deallocation\n");
        break;
    case BLOCK_CREATION:
        ft_printf("New Block Created\n");
        break;
    case BLOCK_ATTRIBUTION:
        ft_printf("Block Attributed\n");
        break;
    case BLOCK_FREED:
        ft_printf("Block Freed\n");
        break;
    case BLOCK_DIVISION:
        ft_printf("Block Divided\n");
        break;
    case MEM_DEFRAG:
        ft_printf("Memory Defragmented\n");
        break;
    }
}