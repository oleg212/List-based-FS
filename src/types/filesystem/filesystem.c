#include "filesystem.h"
#include "../block/block.h"
#include "../block/superblock.h"
#include <string.h>
#include <stdio.h>

void fs_init(filesystem_t* fs) {
    memset(fs, 0, sizeof(filesystem_t));
    init_superblock(fs);
    block_t * root_dir = fs->blocks;
    root_dir->parent = 0;
    root_dir->next = 0;
    root_dir->type = BLOCK_DIRECTORY_HDR;
    strncpy((char*)root_dir->data, "/", MAX_FILENAME);
    
}

block_index fs_create_dir(filesystem_t* fs, block_index parent, const char* name) {
    block_t* dir_it = fs->blocks + parent;

    while (dir_it->next){
        dir_it = fs->blocks + dir_it->next;
        block_index * cells = (block_index *)dir_it->data;
        const uint8_t size = sizeof(dir_it->data) / sizeof(block_index);
        for (uint8_t cell_i = 0; cell_i < size; ++cell_i){
            if (!cells[cell_i])
            {
                cells[cell_i] = allocate_block(fs);
                block_t * new_block  = fs->blocks + cells[cell_i];
                strncpy((char*)new_block->data, name, MAX_FILENAME);
                new_block->type = BLOCK_DIRECTORY_HDR;
                new_block->next=0;
                new_block->parent = dir_it - fs->blocks;
                return cells[cell_i];
            }
        }
    }
    // since we are here, none of dir blocks had empty cells, allocate new one
    dir_it->next = allocate_block(fs);
    dir_it = fs->blocks + dir_it->next;
    dir_it->next = 0;
    dir_it->type = BLOCK_DIRECTORY;
    dir_it->parent = parent;
    block_index * cells = (block_index *)dir_it->data;

    cells[0] = allocate_block(fs);
    block_t * new_block  = fs->blocks + cells[0];
    strncpy((char*)new_block->data, name, MAX_FILENAME);
    new_block->type = BLOCK_DIRECTORY_HDR;
    new_block->next=0;
    new_block->parent = dir_it - fs->blocks;
    return cells[0];
}

block_index fs_create_file(filesystem_t* fs, block_index parent, const char* name) {
    block_t* dir_it = fs->blocks + parent;

    while (dir_it->next){
        dir_it = fs->blocks + dir_it->next;
        block_index * cells = (block_index *)dir_it->data;
        const uint8_t size = sizeof(dir_it->data) / sizeof(block_index);
        for (uint8_t cell_i = 0; cell_i < size; ++cell_i){
            if (!cells[cell_i])
            {
                cells[cell_i] = allocate_block(fs);
                block_t * new_block  = fs->blocks + cells[cell_i];
                strncpy((char*)new_block->data, name, MAX_FILENAME);
                new_block->type = BLOCK_FILE_HDR;
                new_block->next=0;
                new_block->parent = dir_it - fs->blocks;
                return cells[cell_i];
            }
        }
    }
    // since we are here, none of dir blocks had empty cells, allocate new one
    dir_it->next = allocate_block(fs);
    dir_it = fs->blocks + dir_it->next;
    dir_it->next = 0;
    dir_it->type = BLOCK_DIRECTORY;
    dir_it->parent = parent;
    block_index * cells = (block_index *)dir_it->data;

    cells[0] = allocate_block(fs);
    block_t * new_block  = fs->blocks + cells[0];
    strncpy((char*)new_block->data, name, MAX_FILENAME);
    new_block->type = BLOCK_FILE_HDR;
    new_block->next=0;
    new_block->parent = dir_it - fs->blocks;
    return cells[0];
}

void fs_list_dir(filesystem_t* fs, block_index dir_block) {
    block_t* dir = &fs->blocks[dir_block];
    if (dir->type != BLOCK_DIRECTORY_HDR) {
        printf("Not a directory\n");
        return;
    }

    printf("Listing directory [%s](block %u):\n", (char*)dir->data, dir_block);

    block_t* dir_it = dir;
    while (dir_it->next){
        dir_it = fs->blocks + dir_it->next;
        block_index * cells = (block_index *)dir_it->data;
        const uint8_t size = sizeof(dir_it->data) / sizeof(block_index);
        for (uint8_t cell_i = 0; cell_i < size; ++cell_i){
            if (cells[cell_i])
            {
                block_t * content = fs->blocks + cells[cell_i];
                if (content->type == BLOCK_FILE_HDR) {
                    printf("  [File] %s (block %ld)\n", (char*) content->data, content - fs->blocks);
                } else if (content->type == BLOCK_DIRECTORY_HDR) {
                    printf("  [Dir] %s (block %ld)\n", (char*) content->data, content - fs->blocks);
                }
            }
        }
    }
}

void fs_print_free_space(filesystem_t* fs) {
    printf("Free blocks: %u\n", fs->superblock.free_blocks);
}
