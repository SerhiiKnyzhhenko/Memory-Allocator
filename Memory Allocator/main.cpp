#include <iostream>
#include "Allocator.h"
#include <assert.h>
#include "Bump_Allocator.h"

int main() {

    Allocator alloc(1024);

    int* x = (int*)alloc.allocate(sizeof(int), 1);
    double* d = (double*)alloc.allocate(sizeof(double), 1);

    char* ptr_arr = (char*)alloc.allocate(sizeof(char) * 100, 1);

    std::cout << x << ' ' << d << ' ' << (void*)ptr_arr << '\n';

    return 0;
}