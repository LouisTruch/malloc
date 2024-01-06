#ifndef MALLOC_H
#define MALLOC_H

#include <pthread.h> // mutex
#include <stdbool.h>

#include <unistd.h> // getpagesize()
#define PAGE_SIZE getpagesize()
#define TINY_ARENA (size_t)(PAGE_SIZE * 4)                          // 16384 bytes
#define TINY_ALLOC (size_t)(TINY_ARENA / (128 + sizeof(t_chunk)))   // Min 128 alloc/Tiny of 96 bytes max
#define SMALL_ARENA (size_t)(PAGE_SIZE * 64)                        // 262144 bytes
#define SMALL_ALLOC (size_t)(SMALL_ARENA / (128 + sizeof(t_chunk))) // Min 160 alloc/Small of 1632 bytes max

#define MEM_ALLIGN 16
// #define HEAP_SHIFT (((sizeof(t_heap) + MEM_ALLIGN - 1) & ~(MEM_ALLIGN - 1)) - sizeof(t_heap))

// typedef enum e_arena_size
// {
//     TINY,
//     SMALL,
//     LARGE
// } t_arena_size;

typedef struct s_heap
{
    struct s_heap *next, *prev;
    struct s_chunk *chunk;
    // t_arena_size arena_size;
    size_t total_size;
    size_t free_space;
    size_t chunk_count;
} t_heap;

extern t_heap *g_heap;

typedef struct s_chunk
{
    struct s_chunk *next, *prev;
    size_t size;
    bool freed;
} t_chunk;

extern pthread_mutex_t g_mutex;

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

// Heap
t_heap *allocate_new_heap(const size_t arena_size);
t_heap *addback_new_heap(const size_t arena_size);
bool check_heap_free_space(t_heap *heap, const size_t asked_size);
t_heap *search_heap(const size_t size, const size_t arena_size);
t_heap *get_last_heap(void);

// Chunk
void *create_chunk(t_heap **heap, const size_t asked_size);
bool search_chunks(t_heap *heap, const size_t asked_size);
void divide_chunk(t_heap **heap, t_chunk **found_block, const size_t old_block_size);
void *find_free_chunk(t_heap *heap, const size_t asked_size);

// Utils
// int get_arena_size(size_t size);
size_t align_mem(size_t size);

// Log
void show_alloc_mem(void);
void print_heap_info(t_heap *heap);
void show_alloc_mem_hex(void);

// Lib
#include <stdarg.h>

#define B_DEC "0123456789"
#define B_HEXL "0123456789abcdef"
#define B_HEXU "0123456789ABCDEF"

int ft_dprintf(int fd, const char *format, ...);
size_t ft_strlen(const char *s);
void *ft_memmove(void *dest, const void *src, size_t n);
void ft_putstr_fd(char *s, int fd);
void ft_bzero(void *s, size_t n);

// Bonus logger
#define LOGGER_ENV_VAR "MALLOC_LOG"

typedef enum e_logger_state
{
    UNDEFINED,
    DISABLED,
    ENABLED
} t_logger_state;

typedef enum e_log
{
    HEAP_ALLOC,
    HEAP_DEALLOC,
    BLOCK_CREATION,
    BLOCK_ATTRIBUTION,
    BLOCK_FREED,
    BLOCK_DIVISION,
    MEM_DEFRAG,
} t_log;

void logger(int action);

// Record alloc history
#include <errno.h> // open() errno
#include <fcntl.h> // open()

#define HISTORY_FILEPATH "./alloc_history.txt"

typedef enum e_function_call
{
    ALLOC,
    FREE,
} t_function_call;

void record_alloc_history(int function_call, void *ptr);

// Heap_info
#define NC "\033[0m"

#define UWHITE "\033[4;37m"

#define BRED "\033[1;31m"
#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BCYAN "\033[1;36m"
#define BPURPLE "\033[1;35m"

#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define GREEN "\033[0;32m"
#define CYAN "\033[0;36m"

void heap_info(void);

void ptr_hex_dump(void *ptr);

#endif // MALLOC_H