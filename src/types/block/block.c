#include "block.h"

block_index allocate_block(filesystem_t* fs) {
    for (block_index i = 1; i < MAX_BLOCKS; ++i) {
        if (fs->superblock.block_map[i] == 0) {
            fs->superblock.block_map[i] = 1;
            fs->superblock.free_blocks--;
            return i;
        }
    }
    return 0;
}
