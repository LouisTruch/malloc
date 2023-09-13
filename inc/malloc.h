#ifndef MALLOC_H
#define MALLOC_H

#include <sys/mman.h> //mmap() munmap()
#include <unistd.h>   //getpagesize()
#include <stdbool.h>
#include "../src/libft/libft.h"

#include <stdio.h> //REMOVE B4 PUSHING

#define PAGE_SIZE getpagesize()
#define TINY_ARENA (PAGE_SIZE * 4)
#define TINY_ALLOC (TINY_ARENA / (128 + sizeof(t_block)))
#define SMALL_ARENA (PAGE_SIZE * 16)
#define SMALL_ALLOC (SMALL_ARENA / (128 + sizeof(t_block)))

#define MEM_ALLIGN 16
#define HEAP_SHIFT (((sizeof(t_heap) + MEM_ALLIGN - 1) & ~(MEM_ALLIGN - 1)) - sizeof(t_heap))

typedef enum e_arena_size
{
    TINY,
    SMALL,
    LARGE
} t_arena_size;

typedef struct s_heap
{
    struct s_heap *next, *prev;
    struct s_block *block;
    t_arena_size arena_size;
    size_t total_size;
    size_t free_space;
    size_t block_count;
} t_heap;

extern t_heap *g_heap;

typedef struct s_block
{
    struct s_block *next, *prev;
    size_t size;
    bool freed;
} t_block;

typedef enum e_log
{
    HEAP_ALLOC,
    HEAP_DEALLOC,
    BLOCK_CREATION,
    BLOCK_ATTRIBUTION,
    BLOCK_RELEASE,
    MEM_DEFRAG,
    BLOCK_DIVISION
} t_log;

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

// Utils
int get_arena_size(size_t size);
size_t align_mem(size_t size);

void logger(int debug);
void show_alloc_mem(void);

#endif // MALLOC_H