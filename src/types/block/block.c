#include "block.h"
#include <stdlib.h>

static block_index  allocate_block(filesystem_t* fs) {
    for (block_index i = 0; i < MAX_BLOCKS; ++i) {
        if (fs->superblock.block_map[i] == 0) {
            fs->superblock.block_map[i] = 1;
            fs->superblock.free_blocks--;
            return i;
        }
    }
    return INVALID_BLOCK;
}

block_index allocate_and_init_block(filesystem_t* fs, block_type type, block_index parent, const char* name) {
    block_index index = allocate_block(fs);
    if (index == INVALID_BLOCK) return NULL;

    block_t* block = get_block(fs, index);
    block->type = type;
    block->next = INVALID_BLOCK;
    block->parent = parent;
    if (name) {
        strncpy((char*)block->data, name, MAX_FILENAME);
    }
    return index;
}

block_index get_block_index(filesystem_t* fs, block_t* block)
{
    return block - fs->blocks;
}

block_t* get_block(filesystem_t* fs, block_index index) {
    if (index < 0 || index >= MAX_BLOCKS) {
        return NULL;
    }
    return &fs->blocks[index];
}

block_t* get_next_block(filesystem_t* fs, block_index index) {
    return get_block(fs, get_block(fs, index)->next);
}

block_t* get_parent_block(filesystem_t* fs, block_index index) {
    return get_block(fs, get_block(fs, index)->parent);
}