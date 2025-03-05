#include "superblock.h"
#include <string.h>

void init_superblock(filesystem_t* fs) {
    fs->superblock.total_blocks = MAX_BLOCKS;
    fs->superblock.free_blocks = MAX_BLOCKS;
}
