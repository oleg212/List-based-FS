#include "filesystem.h"
#include "../block/block.h"
#include "../block/superblock.h"
#include <string.h>
#include <stdio.h>

void fs_init(filesystem_t* fs) {
    memset(fs, 0, sizeof(filesystem_t));
    init_superblock(fs);
    fs->blocks[0].type = BLOCK_SUPERBLOCK;
}

block_index fs_create_dir(filesystem_t* fs, block_index parent) {
    block_index dir_block = allocate_block(fs);
    if (dir_block == 0) return 0;

    block_t* block = &fs->blocks[dir_block];
    block->type = BLOCK_DIRECTORY;
    block->parent = parent;
    block->next = 0;

    return dir_block;
}

block_index fs_create_file(filesystem_t* fs, block_index parent, const char* name) {
    block_index file_block = allocate_block(fs);
    if (file_block == 0) return 0;

    block_t* block = &fs->blocks[file_block];
    block->type = BLOCK_FILE;
    block->parent = parent;
    block->next = 0;

    strncpy((char*)block->data, name, MAX_FILENAME);

    return file_block;
}

void fs_list_dir(filesystem_t* fs, block_index dir_block) {
    block_t* dir = &fs->blocks[dir_block];
    if (dir->type != BLOCK_DIRECTORY) {
        printf("Not a directory\n");
        return;
    }

    printf("Listing directory (block %u):\n", dir_block);

    for (block_index i = 1; i < MAX_BLOCKS; ++i) {
        block_t* block = &fs->blocks[i];
        if (block->parent == dir_block) {
            if (block->type == BLOCK_FILE) {
                printf("  [File] %s (block %u)\n", (char*)block->data, i);
            } else if (block->type == BLOCK_DIRECTORY) {
                printf("  [Dir] <unnamed> (block %u)\n", i);
            }
        }
    }
}

void fs_print_free_space(filesystem_t* fs) {
    printf("Free blocks: %u\n", fs->superblock.free_blocks);
}
