#include "block.h"
#include <stdlib.h>

block_index  allocate_block(filesystem_t* fs) {
    for (block_index i = 1; i < MAX_BLOCKS; ++i) {
        if (fs->superblock.block_map[i] == 0) {
            fs->superblock.block_map[i] = 1;
            fs->superblock.free_blocks--;
            return i;
        }
    }
    return NULL;
}

block_t * goto_next(block_t * block){
    if (block->next)
        return block + block->next;
    else
        return NULL;
}

block_t * goto_parent(block_t * block){
    if (block->parent)
        return block + block->parent;
    else
        return NULL;

}
