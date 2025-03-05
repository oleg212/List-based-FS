#include "filesystem.h"
#include "../block/block.h"
#include "../block/superblock.h"
#include <string.h>
#include <stdio.h>

void fs_init(filesystem_t* fs) {
    memset(fs, 0, sizeof(filesystem_t));
    init_superblock(fs);
    allocate_and_init_block(fs, BLOCK_DIRECTORY_HDR, 0, "/");
}

block_index fs_create_entry(filesystem_t* fs, block_index parent, const char* name, entry_type type) {
    block_t* dir = get_block(fs, parent);

    if (!dir)
    {
        return INVALID_BLOCK;
    }

    while (dir->next != INVALID_BLOCK) {
        dir = get_next_block(fs, get_block_index(fs, dir));
        block_index* cells = (block_index*)dir->data;
        size_t cell_count = sizeof(dir->data) / sizeof(block_index);

        for (size_t i = 0; i < cell_count; ++i) {
            if (cells[i] == 0) {
                block_index new_entry;

                if (type == E_FILE)
                    new_entry = allocate_and_init_block(fs, BLOCK_FILE_HDR, get_block_index(fs, dir), name);
                else if (type == E_DIR)
                    new_entry = allocate_and_init_block(fs, BLOCK_DIRECTORY_HDR, get_block_index(fs, dir), name);
                else
                    return INVALID_BLOCK;

                cells[i] = (block_index)(new_entry);
                return new_entry;
            }
        }
    }

    // Если нет места — создаем новый блок каталога
    block_index new_block = allocate_and_init_block(fs, BLOCK_DIRECTORY, parent, NULL);
    dir->next = (block_index)(new_block);

    block_index new_entry;
    
    if (type == E_FILE)
        new_entry = allocate_and_init_block(fs, BLOCK_FILE_HDR, new_block, name);
    else if (type == E_DIR)
        new_entry = allocate_and_init_block(fs, BLOCK_DIRECTORY_HDR, new_block, name);
    else
        return INVALID_BLOCK;
    (get_block(fs, new_block)->data)[0] = new_entry;

    return new_entry;
}


void fs_list_dir(filesystem_t* fs, block_index dir_block) {
    if (dir_block == INVALID_BLOCK)
    {
        printf("fs_list_dir(): ERROR null index\n");
        return;
    }

    block_t* dir = get_block(fs, dir_block);
    if (dir->type != BLOCK_DIRECTORY_HDR)
    {
        printf("fs_list_dir(): ERROR not a dir\n");
        return;
    }

    printf("Listing directory [%s](block %u):\n", (char*)dir->data, dir_block);

    while (dir->next != INVALID_BLOCK) {
        dir = get_next_block(fs, get_block_index(fs, dir));
        block_index* cells = (block_index*)dir->data;
        size_t cell_count = sizeof(dir->data) / sizeof(block_index);
        for (uint8_t cell_i = 0; cell_i < cell_count; ++cell_i){
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
