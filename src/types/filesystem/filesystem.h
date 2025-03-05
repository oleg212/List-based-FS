#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

#define BLOCK_SIZE 64
#define MAX_FILENAME 16
#define MAX_BLOCKS 128

typedef uint8_t byte;
typedef uint32_t block_index;

#define INVALID_BLOCK (block_index) (-1)

typedef enum {
    BLOCK_UNUSED = 0,
    BLOCK_SUPERBLOCK,
    BLOCK_DIRECTORY,
    BLOCK_DIRECTORY_HDR,
    BLOCK_FILE,
    BLOCK_FILE_HDR
} block_type;

typedef enum {
    E_FILE,
    E_DIR
} entry_type;

typedef struct {
    #ifdef MEM_LABELS
    char label[8];
    #endif
    block_type type;
    block_index next;
    block_index parent;
    #ifdef MEM_LABELS
    byte data[BLOCK_SIZE - sizeof(block_type) - 2 * sizeof(block_index) - sizeof(label)];
    #else
    byte data[BLOCK_SIZE - sizeof(block_type) - 2 * sizeof(block_index)];
    #endif
} block_t;

typedef struct {
    #ifdef MEM_LABELS
    char label[8];
    #endif
    uint32_t total_blocks;
    uint32_t free_blocks;
    byte block_map[MAX_BLOCKS];
} superblock_t;

typedef struct {
    block_t blocks[MAX_BLOCKS];
    superblock_t superblock;
} filesystem_t;

void fs_init(filesystem_t* fs);
block_index fs_create_entry(filesystem_t* fs, block_index parent, const char* name, entry_type type);
void fs_list_dir(filesystem_t* fs, block_index dir_block);
void fs_print_free_space(filesystem_t* fs);

#endif
