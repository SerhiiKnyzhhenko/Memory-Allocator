#include "Allocator.h"

#define HEADER_SIZE sizeof(BlockHeader)
#define ALIGNMENT 16
#define MIN_PAYLOAD_SIZE 8
#define MIN_BLOCK_SIZE (align_up(sizeof(BlockHeader) + MIN_PAYLOAD_SIZE, ALIGNMENT))

static BlockHeader* head = NULL;
static BlockHeader* tail = NULL;  

typedef struct BlockHeader {
    size_t size;
    struct BlockHeader* next;
    bool is_free;
} BlockHeader;


void* myAlloc::malloc(size_t size) {
    if (size == 0) return NULL;

    BlockHeader* block = find_free_block(size);

    if (block) {
        block->is_free = false;
        return (void*)(block + 1);
    }

    return allocate_new_block(size);

}

BlockHeader* find_free_block(size_t size) {
    BlockHeader* current = head;
    while (current) {
        if (current->is_free && current->size >= size + HEADER_SIZE) {
            if (current->size >= size + HEADER_SIZE + MIN_BLOCK_SIZE) {
                split_block(current, size);
            }
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void split_block(BlockHeader* block, size_t size) {

}

size_t align_up(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

void* allocate_new_block(size_t size) {
  
}


void myAlloc::free(void* ptr) {

}

size_t myAlloc::allocated_bytes() {

}