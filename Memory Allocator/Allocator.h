#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#define HEADER_SIZE sizeof(BlockHeader)
#define ALIGNMENT 16
#define MIN_PAYLOAD_SIZE 8
#define MIN_BLOCK_SIZE (align_up(sizeof(BlockHeader) + MIN_PAYLOAD_SIZE, ALIGNMENT))

#include <stdio.h>
#include <Windows.h>

struct BlockHeader;

extern struct BlockHeader* head;
extern struct BlockHeader* tail;

namespace myAlloc {
    void* malloc(size_t size);
    void free(void* ptr);
    size_t allocated_bytes(void* ptr);
}

BlockHeader* find_free_block(size_t size);
void split_block(BlockHeader* block, size_t requested_size);
void* allocate_new_block(size_t size);
void* request_mmemory(size_t size);
size_t align_up(size_t size, size_t alignment);
void collapse_block(BlockHeader* header);

#endif // !ALLOCATOR_H



