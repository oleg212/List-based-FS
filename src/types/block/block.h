#ifndef BLOCKS_H
#define BLOCKS_H

#include "../filesystem/filesystem.h"

block_index allocate_block(filesystem_t* fs);
block_t * goto_next(block_t * block);
block_t * goto_parent(block_t * block);

#endif
