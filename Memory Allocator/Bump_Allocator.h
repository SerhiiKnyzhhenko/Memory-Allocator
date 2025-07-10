#ifndef BUMP_ALLOCATOR
#define BUMP_ALLOCATOR

#include <cstddef> // Для size_t
#include <cstdint> // Для uintptr_t
#include <iostream>
#include "Block.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

class Allocator {
public:
    Allocator(size_t size) {

        size_t alignment = 16;
        m_totalSize = (size + alignment - 1) & ~(alignment - 1);

        #ifdef _WIN32
        m_start = VirtualAlloc(NULL, m_totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        #else
        m_start = mmap(nullptr, m_totalSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        #endif

        m_current = m_start;

        Block* initial_block = static_cast<Block*>(m_start);
        initial_block->size_ = m_totalSize;
        initial_block->is_free_ = true;
        initial_block->free_block_pointers.next_free = nullptr;
        initial_block->free_block_pointers.prev_free = nullptr;
    
        m_free_list_head = initial_block;
    }

    ~Allocator() {

        #ifdef _WIN32
        VirtualFree(m_start, 0, MEM_RELEASE);
        #else
        munmap(m_start, m_totalSize);
        #endif

    }

    void* allocate(size_t requested_size) {
    
        Block* current_block = m_free_list_head;
        while (current_block != nullptr) {
            if (current_block->size_ >= requested_size) {
                break;
            }
            current_block = current_block->free_block_pointers.next_free;
        }

        if (current_block == nullptr)
            return nullptr;
        
        if (current_block->size_ >= requested_size + sizeof(Block)) {
            size_t allocated_size = requested_size + sizeof(Block);
            size_t new_block_size = current_block->size_ - allocated_size;

            Block* new_block = reinterpret_cast<Block*>(reinterpret_cast<uintptr_t>(current_block) + allocated_size);
            new_block->size_ = new_block_size;
            new_block->is_free_ = true;

            current_block->size_ = allocated_size;
            current_block->is_free_ = false;

            Block* next_block = current_block->free_block_pointers.next_free;
            Block* prev_block = current_block->free_block_pointers.prev_free;

            if (prev_block != nullptr) {
                prev_block->free_block_pointers.next_free = new_block;
            }
            else {
                m_free_list_head = new_block;
            }

            if (next_block != nullptr)
                next_block->free_block_pointers.prev_free = new_block;


            new_block->free_block_pointers.prev_free = prev_block;
            new_block->free_block_pointers.next_free = next_block;

            return (void*)current_block->user_data;
        }
        else {
            Block* next_block = current_block->free_block_pointers.next_free;
            Block* prev_block = current_block->free_block_pointers.prev_free;

            if (prev_block != nullptr) {
                prev_block->free_block_pointers.next_free = next_block;
            }
            else {
                m_free_list_head = next_block;
            }

            if (next_block != nullptr)
                next_block->free_block_pointers.prev_free = prev_block;

            current_block->is_free_ = false;
            return (void*)current_block->user_data;
        }
    }

    void free(void* user_data_ptr) {
        Block* current_block = reinterpret_cast<Block*>(
            reinterpret_cast<uintptr_t>(user_data_ptr)  - offsetof(Block, user_data)
        );

        current_block->is_free_ = true;

        Block* right_block = reinterpret_cast<Block*>(
            reinterpret_cast<uintptr_t>(current_block) + current_block->size_
        );

        if (reinterpret_cast<uintptr_t>(right_block) < reinterpret_cast<uintptr_t>(m_start) + m_totalSize
            && right_block->is_free_ == true) {

            Block* next_block = right_block->free_block_pointers.next_free;
            Block* prev_block = right_block->free_block_pointers.prev_free;

            if (prev_block != nullptr) {
                prev_block->free_block_pointers.next_free = next_block;
            }
            else {
                m_free_list_head = next_block;
            }

            if (next_block != nullptr)
                next_block->free_block_pointers.prev_free = prev_block;

            current_block->size_ += right_block->size_;
        }

        current_block->free_block_pointers.next_free = m_free_list_head;
        current_block->free_block_pointers.prev_free = nullptr;

        if(m_free_list_head != nullptr)
            m_free_list_head->free_block_pointers.prev_free = current_block;
            
        m_free_list_head = current_block;

    }

private:
    Block* m_free_list_head = nullptr;
    void* m_start = nullptr;
    void* m_current = nullptr;
    size_t m_totalSize = 0;
};

#endif // !BUMP_ALLOCATOR
