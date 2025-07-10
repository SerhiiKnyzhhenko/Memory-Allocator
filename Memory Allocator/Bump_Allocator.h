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
        // 1. Запроси 'size' байт памяти у ОС.
        //    Используй mmap для Linux/macOS или VirtualAlloc для Windows.
        //    Сохрани указатель на начало в m_start.
        //    Инициализируй m_current с тем же адресом.

        size_t alignment = 16;
        m_totalSize = (size + alignment - 1) & ~(alignment - 1);

        #ifdef _WIN32
        m_start = VirtualAlloc(NULL, m_totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        #else
        // Аналог для Linux/macOS
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
        // 2. Освободи большой кусок памяти, который мы получили от ОС.
        //    Используй munmap для Linux/macOS или VirtualFree для Windows.
        #ifdef _WIN32
        VirtualFree(m_start, 0, MEM_RELEASE);
        #else
        munmap(m_start, m_totalSize);
        #endif

    }

    void* allocate(size_t requested_size) {
    
        Block* current = m_free_list_head;
        while (current != nullptr) {
            if (current->size_ >= requested_size) {
                break;
            }
            current = current->free_block_pointers.next_free;
        }

        if (current == nullptr)
            return nullptr;
        
        if (current->size_ >= requested_size + sizeof(Block)) {
            size_t allocated_size = requested_size + sizeof(Block);
            size_t new_block_size = current->size_ - allocated_size;

            Block* new_block = reinterpret_cast<Block*>(reinterpret_cast<uintptr_t>(current) + allocated_size);
            new_block->size_ = new_block_size;
            new_block->is_free_ = true;

            current->size_ = allocated_size;
            current->is_free_ = false;

            Block* next_block = current->free_block_pointers.next_free;
            Block* prev_block = current->free_block_pointers.prev_free;

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

            return (void*)current->user_data;
        }
        else {
            Block* next_block = current->free_block_pointers.next_free;
            Block* prev_block = current->free_block_pointers.prev_free;

            if (prev_block != nullptr) {
                prev_block->free_block_pointers.next_free = next_block;
            }
            else {
                m_free_list_head = next_block;
            }

            if (next_block != nullptr)
                next_block->free_block_pointers.prev_free = prev_block;

            current->is_free_ = false;
            return (void*)current->user_data;
        }
    }

private:
    Block* m_free_list_head = nullptr;
    void* m_start = nullptr;
    void* m_current = nullptr;
    size_t m_totalSize = 0;
};

#endif // !BUMP_ALLOCATOR
