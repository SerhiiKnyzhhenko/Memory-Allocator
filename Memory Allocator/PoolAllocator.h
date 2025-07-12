#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include "IAllocator.h"
#include <cstddef> // Для size_t
#include <cstdint> 

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

struct Chunk {
	union {
		Chunk* next_free;
		char user_data[1];
	};
};

class PoolAllocator : public IAllocator {

private:
    size_t m_totalSize_;
    size_t chunk_size_;
    size_t num_chunks_;
	void* m_start_ = nullptr;
	void* m_current_ = nullptr;
	Chunk* m_free_list_head = nullptr;

public:
	PoolAllocator(size_t chunk_size, size_t num_chunks);
	~PoolAllocator();

	void* allocate(size_t size) override;
	void deallocate(void* data) override;

private:
    
};

#endif // !POOLALLOCATOR_H



