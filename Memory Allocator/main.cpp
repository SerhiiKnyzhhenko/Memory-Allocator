#include <iostream>
#include "Allocator.h"
#include <assert.h>
#include "Bump_Allocator.h"



int main() {

    Allocator alloc(1024);

    void* ptr1 = alloc.allocate(150);
    void* ptr2 = alloc.allocate(250);
    void* ptr3 = alloc.allocate(350);

    alloc.free(ptr1);
    alloc.free(ptr2);
    alloc.free(ptr3);

    return 0;
}