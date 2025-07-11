#ifndef GENERALPURPOSEALLOCATOR_H
#define GENERALPURPOSEALLOCATOR_H

#include "IAllocator.h"
#include <cstddef> // Для size_t
#include <cstdint> // Для uintptr_t
#include <iostream>
#include "Block.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

class GeneralPurposeAllocator : public IAllocator {

private:
    Block* m_free_list_head = nullptr;
    void* m_start = nullptr;
    void* m_current = nullptr;
    size_t m_totalSize = 0;

public:
    GeneralPurposeAllocator(size_t size);
    ~GeneralPurposeAllocator();

    void* allocate(size_t requested_size);
    void deallocate(void* user_data_ptr);

};

#endif // !GENERALPURPOSEALLOCATOR_H
