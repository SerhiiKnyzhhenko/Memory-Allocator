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

void split_block(BlockHeader* block, size_t requested_size) {
    size_t total_neaded = requested_size + sizeof(BlockHeader);
    size_t remaining_size = block->size - total_neaded;

    if (remaining_size >= MIN_BLOCK_SIZE) {
        BlockHeader* new_block = (BlockHeader*)((char*)block + total_neaded);
        new_block->size = remaining_size;
        new_block->next = block->next;
        new_block->is_free = true;
        block->size = total_neaded;
        block->next = new_block;
    }   
}


void* allocate_new_block(size_t size) {
    
    size_t total_size = size + HEADER_SIZE;

    void* mem = request_mmemory(size);

    if (!mem) return NULL;

    BlockHeader* header = (BlockHeader*)mem;

    header->next = NULL;
    header->size = total_size;
    header->is_free = true;

    if (!head) head = header;
    if (tail) tail = header->next;
    tail = header;

    return (void*)(header + 1);
}

void* request_mmemory(size_t size) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    size_t alligned_size = align_up(size, si.dwPageSize);

    LPVOID ptr = VirtualAlloc(
        NULL,
        alligned_size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    return ptr;
}

size_t align_up(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

void myAlloc::free(void* ptr) {
    if (!ptr) return;

    BlockHeader* header = (BlockHeader*)ptr - 1;
    header->is_free = true;

    collapse_block(header);
}

void collapse_block(BlockHeader* header) {

    if (header->next && header->next->is_free) {
        header->size += header->next->size;
        header->next = header->next->next;
        if (header->next == NULL) tail = header;
    }
}

size_t myAlloc::allocated_bytes() {

}