#include "../inc/malloc.h"
#include <stdlib.h> // getenv()

static void print_action(char *action)
{
    ft_putstr_fd(action, 1);
}

static u_int8_t check_env_var(void)
{
    char *env_var_logger = getenv(LOGGER_ENV_VAR);
    if (env_var_logger && (!ft_strcmp(env_var_logger, "yes") || !ft_strcmp(env_var_logger, "true") ||
                           !ft_strcmp(env_var_logger, "enabled")))
    {
        char *env_var_logger_level = getenv(LOGGER_LEVEL_ENV_VAR);
        if (!env_var_logger_level || (env_var_logger_level && (!ft_strcmp(env_var_logger_level, "basic"))))
        {
            print_action("Logger enabled: Level Basic");
            return BASIC;
        }
        if (env_var_logger_level && (!ft_strcmp(env_var_logger_level, "advanced")))
        {
            print_action("Logger enabled: Level Advanced");
            return ADVANCED;
        }
        if (env_var_logger_level && (!ft_strcmp(env_var_logger_level, "full")))
        {
            print_action("Logger enabled: Level Full");
            return FULL;
        }
    }
    return DISABLED;
}

void logger(enum e_log_action action, void *ptr)
{
    static u_int8_t logger_state = UNDEFINED;
    if (logger_state == DISABLED)
        return;
    if (logger_state == UNDEFINED)
    {
        logger_state = check_env_var();
        if (logger_state == DISABLED)
            return;
        write(1, "\n", 1);
    }

    switch (action)
    {
    case CALL_MALLOC:
        print_action("Call Malloc\n");
        return;
    case CALL_FREE:
        print_action("Call Free\n");
        return;
    default:
        break;
    }
    if (logger_state >= ADVANCED)
        switch (action)
        {
        case HEAP_ALLOC:
            print_action("New Heap Allocation");
            break;
        case HEAP_DEALLOC:
            print_action("Heap Deallocation");
            break;
        case CHUNK_CREATION:
            print_action("New Chunk Created");
            break;
        case CHUNK_ATTRIBUTION:
            print_action("Chunk Attributed");
            break;
        case CHUNK_FREED:
            print_action("Chunk Freed");
            break;
        case CHUNK_DIVISION:
            print_action("Chunk Divided");
            break;
        case MEM_DEFRAG:
            print_action("Memory Defragmented");
            break;
        default:
            break;
        }
    if (logger_state == FULL && ptr)
        ft_dprintf(1, "\t@: %p", ptr);
    if (logger_state != BASIC)
        write(1, "\n", 1);
}
// switch (action)
// {
// case HEAP_ALLOC:
//     ft_dprintf(1, "New Heap Allocation\n");
//     break;
// case HEAP_DEALLOC:
//     ft_dprintf(1, "Heap Deallocation\n");
//     break;
// case BLOCK_CREATION:
//     ft_dprintf(1, "New Block Created\n");
//     break;
// case BLOCK_ATTRIBUTION:
//     ft_dprintf(1, "Block Attributed\n");
//     break;
// case BLOCK_FREED:
//     ft_dprintf(1, "Block Freed\n");
//     break;
// case BLOCK_DIVISION:
//     ft_dprintf(1, "Block Divided\n");
//     break;
// case MEM_DEFRAG:
//     ft_dprintf(1, "Memory Defragmented\n");
//     break;
// }