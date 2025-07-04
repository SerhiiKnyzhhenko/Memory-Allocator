#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <Windows.h>

namespace myAlloc {

    void* malloc(size_t size);
    void free(void* ptr);
    size_t allocated_bytes();

}

#endif // !ALLOCATOR_H



