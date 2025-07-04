#include "Allocator.h"

#define HEADER_SIZE sizeof(BlockHeader)

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

}

void* allocate_new_block(size_t size) {
  
}


void myAlloc::free(void* ptr) {

}

size_t myAlloc::allocated_bytes() {

}