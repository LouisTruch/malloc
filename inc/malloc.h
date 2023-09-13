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

typedef enum e_alloc_type
{
    FREE_BLOCK,
    FREE_SPACE
} t_alloc_type;

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

void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

// Heap
t_heap *initialize_heap(int arena_range, size_t requested_size);
t_heap *search_adapted_heap(int arena_range, size_t requested_size);

// Block
void *get_block_address(t_heap *heap, size_t requested_size);

// Free
void my_free(void *ptr);

// Utils
int get_arena_size(size_t size);
size_t align_mem(size_t size);

void logger(int debug);
void debug_a(void *ptr);
void show_alloc_mem(void);

// Tests, delete before pushing
void linked_list(t_heap **heap);

#endif // MALLOC_H