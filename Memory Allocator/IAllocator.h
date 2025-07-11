#ifndef AIALLOCATOR_H
#define AIALLOCATOR_H

class IAllocator
{
public:
	virtual ~IAllocator() = default;

	virtual void* allocate(size_t size) = 0;
	virtual void deallocate(void* data) = 0;
};

#endif // !AIALLOCATOR_H



