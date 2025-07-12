#ifndef BLOCK_H
#define BLOCK_H

struct Block {
	size_t size_;
	bool is_free_;

	union {
		struct {
			Block* next_free;
			Block* prev_free;
		}free_block_pointers;
		char user_data[1];
	};	
};

#endif //BLOCK_H




