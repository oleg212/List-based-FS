#ifndef BLOCKS_H
#define BLOCKS_H

#include "../filesystem/filesystem.h"

block_index allocate_and_init_block(filesystem_t* fs, block_type type, block_index parent, const char* name);

block_index get_block_index(filesystem_t* fs, block_t* block);
block_t* get_block(filesystem_t* fs, block_index index);
block_t* get_next_block(filesystem_t* fs, block_index index);
block_t* get_parent_block(filesystem_t* fs, block_index index);

#endif
