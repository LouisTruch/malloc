#include "../inc/malloc.h"

static void *add_new_block(t_heap *heap, size_t requested_size)
{
    logger(BLOCK_CREATION);
    void *ptr = heap + 1;
    ft_printf("&heap:%X &ptr:%X\n", heap, ptr);
    t_block *block_ptr = heap->block, *head_block = heap->block;
    // If no head block
    if (block_ptr == NULL)
    {
        block_ptr = ptr;
        block_ptr->size = requested_size + sizeof(t_block);
        block_ptr->freed = false;
        block_ptr->next = NULL;
        block_ptr->prev = NULL;
        heap->block = block_ptr;
        heap->block_count += 1;
        heap->free_space -= sizeof(t_block) - requested_size;
        block_ptr += 1;
        ft_printf("FirstBlockEnd:%X\n", block_ptr);
        return block_ptr;
    }
    while (block_ptr != NULL)
    {
        ptr = block_ptr;
        // Can move this below out of loop
        ft_printf("block_size:%i\n", block_ptr->size);
        if (block_ptr->next == NULL) {
            for (size_t block_size = 0 ; block_size != block_ptr->size ; block_size++)
            {
                ptr++;
            }
            heap->block_count += 1;
            heap->free_space -= sizeof(t_block) - requested_size;
            t_block *new_block = ptr;
            block_ptr->next = ptr;
            new_block->prev = block_ptr;
            new_block->next = NULL;
            new_block->freed = false;
            new_block->size= requested_size + sizeof(t_block);
            for (size_t struct_size = 0 ; struct_size != sizeof(t_block) ; struct_size++)
            {
                ptr++;
            }
            break;
        }
        block_ptr = block_ptr->next;
    }
    g_heap = head_block;
    ft_printf("&ptr end:%X\n", ptr);
    return ptr;
}

static void *search_freed_blocks(t_heap *heap, size_t requested_size)
{
    t_block *block = heap->block;
    while(block != NULL)
    {
        if (block->freed && block->size >= requested_size + sizeof(t_block))
        {
            // Have to divide blocks also and relink them and change t_block struct
            logger(BLOCK_ATTRIBUTION);
            return block;
        }
        block = block->next;
    }
    return NULL;
}

void *get_block_address(t_heap *heap, size_t requested_size)
{
    void *ptr;
    if (!(ptr = search_freed_blocks(heap, requested_size)))
        ptr = add_new_block(heap, requested_size);
    debug_a(ptr);
    return ptr;
}