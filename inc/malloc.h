#ifndef MALLOC_H
#define MALLOC_H

#include <sys/mman.h> //mmap() munmap()
#include <unistd.h> //getpagesize()
#include <stdbool.h>
#include "../src/libft/libft.h"


#include <stdio.h> //REMOVE B4 PUSHING

#define PAGE_SIZE getpagesize()
#define TINY_ARENA (PAGE_SIZE * 4)
#define TINY_ALLOC (TINY_ARENA / (128 + sizeof(t_block))) 
#define SMALL_ARENA (PAGE_SIZE * 16)
#define SMALL_ALLOC (SMALL_ARENA / (128 + sizeof(t_block)))

typedef enum e_heap_size {
    TINY,
    SMALL,
    LARGE
} t_heap_size;


typedef struct s_heap {
    struct s_heap *next, *prev;
    struct s_block *block;
    t_heap_size arena_size;
    size_t free_space;
    size_t block_count;
} t_heap; // size ?

extern t_heap *g_heap;

typedef struct s_block {
    struct s_block *next, *prev;
    size_t size;
    bool freed;
} t_block; // 32B

void *my_malloc(size_t size);

#endif // MALLOC_H