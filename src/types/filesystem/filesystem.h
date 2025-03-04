#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

#define BLOCK_SIZE 64
#define MAX_FILENAME 16
#define MAX_BLOCKS 128

typedef uint8_t byte;
typedef uint32_t block_index;

typedef enum {
    BLOCK_UNUSED = 0,
    BLOCK_SUPERBLOCK,
    BLOCK_DIRECTORY,
    BLOCK_FILE
} block_type;

typedef struct {
    block_type type;
    block_index next;
    block_index parent;
    byte data[BLOCK_SIZE - sizeof(block_type) - 2 * sizeof(block_index)];
} block_t;

typedef struct {
    uint32_t total_blocks;
    uint32_t free_blocks;
    byte block_map[MAX_BLOCKS];
} superblock_t;

typedef struct {
    block_t blocks[MAX_BLOCKS];
    superblock_t superblock;
} filesystem_t;

void fs_init(filesystem_t* fs);
block_index fs_create_dir(filesystem_t* fs, block_index parent);
block_index fs_create_file(filesystem_t* fs, block_index parent, const char* name);
void fs_list_dir(filesystem_t* fs, block_index dir_block);
void fs_print_free_space(filesystem_t* fs);

#endif
